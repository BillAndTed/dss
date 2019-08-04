/*
    ImageCache.cpp
*/
#include "ImageCache.h"

std::unordered_map<std::string, std::shared_ptr<CachedImage> > ImageCache::_lookup;
std::deque<std::shared_ptr<CachedImage>> ImageCache::_loadQueue;
std::list<std::shared_ptr<CachedImage>> ImageCache::_cache;

void CachedImage::LoadAsync()
{
    _image->load(_URL);
    _fetching = true;
    std::string URL = _URL;
    _image->addEventListener(HttpRequestTask::COMPLETE, [URL](Event* event){

        auto cachedImage = ImageCache::_lookup[URL];

        logs::messageln("Async fetch of asset (%s) COMPLETE.", URL.c_str());

        cachedImage->_fetching = false;
        cachedImage->_fetched = true;
        cachedImage->_error = false;
        if(cachedImage->_onSuccess)
            cachedImage->_onSuccess(cachedImage->_image);
    });

    _image->addEventListener(HttpRequestTask::ERROR, [URL](Event* event){
        auto cachedImage = ImageCache::_lookup[URL];
        cachedImage->_fetching = false;
        cachedImage->_fetched = false;
        cachedImage->_error = true;
        if(cachedImage->_onError)
            cachedImage->_onError(0, URL);
    });
}

void ImageCache::init()
{

}
void ImageCache::update()
{
    // TODO: we could have N network requests in-flight at a given time (decrease load time)
    // but i'm limiting it here to just 1

    // If we have things to fetch in the queue, do so
    if(ImageCache::_loadQueue.size())
    {
        auto asset = ImageCache::_loadQueue.front();
        if(asset->fetched())
        {
            // done. kick off the next one next update
            ImageCache::_loadQueue.pop_front();
        }
        else if(!asset->isFetching())
        {
            // kick off fetching new assets
            asset->LoadAsync();
        }
    }
}

void ImageCache::release()
{
    for(auto q:ImageCache::_loadQueue)
    {
        q->_image->removeAllEventListeners();
    }
    ImageCache::_loadQueue.clear();
    ImageCache::_lookup.clear();
    ImageCache::_cache.clear();
}

void ImageCache::updateCache()
{
    ImageCache::_cache.sort([](const std::shared_ptr< CachedImage > & a, const std::shared_ptr< CachedImage > & b)->bool
    {
        return a->_lastAccess < b->_lastAccess;
    });
    
    // would probably be better to limit cache size on available memory or other
    // but this is good enough for now
    const static size_t MAX_NUM_IMAGES = 128;
    while(ImageCache::_cache.size() > MAX_NUM_IMAGES)
    {
        std::shared_ptr<CachedImage> img = ImageCache::_cache.back();
        ImageCache::_lookup.erase(img->_URL);
        ImageCache::_cache.pop_back();
    }
}

spWebImage ImageCache::get(const std::string& URL,
    std::function<void(spWebImage)> onSuccess,
    std::function<void(int,const std::string&)> onError)
{
    auto img = ImageCache::_lookup.find(URL);
    if(img != ImageCache::_lookup.end())
    {
        // Update the access time for this asset, and sort, most recent accessed to last
        img->second->_lastAccess = std::time(nullptr);
        ImageCache::updateCache();

        logs::messageln("Providing cached image at URL:%s\n", URL.c_str());
        // we have the asset in cache. Invoke success callback immediately
        img->second->_fetched = true;
        onSuccess(img->second->_image);
        return img->second->_image;
    }

    spWebImage newImg = new WebImage;
    auto cachedImage = std::make_shared<CachedImage>(URL, newImg, onSuccess, onError);
    ImageCache::_lookup[URL] = cachedImage;
    // queue the asset for async load
    ImageCache::_loadQueue.push_back(cachedImage);
    ImageCache::_cache.push_back(cachedImage);

    updateCache();

    return newImg;
}
    