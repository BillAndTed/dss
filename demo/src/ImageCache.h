/*
    ImageCache.h
    Cache images fetched off the web to some upper resource/use limit
*/
#pragma once

#include "oxygine-framework.h"

using namespace oxygine;

#include <functional>
#include <unordered_set>
#include <deque>

struct CachedImage
{
    std::string _URL;
    spWebImage _image;
    bool _fetching = false;
    bool _fetched = false;
    bool _error = false;
    std::function<void(spWebImage)> _onSuccess = nullptr;
    std::function<void(int,const std::string&)> _onError = nullptr;

    CachedImage(const std::string& URL, spWebImage image,
        std::function<void(spWebImage)> onSuccess = nullptr,
        std::function<void(int,const std::string&)> onError = nullptr)
    :_URL(URL)
    ,_image(image)
    ,_onSuccess(onSuccess)
    ,_onError(onError){};

    void LoadAsync();

    bool isFetching() const { return _fetching;}
    bool fetched() const { return _fetched;}
    bool error() const { return _error;};

};

class ImageCache
{
public:
    static void init();
    static void update();
    static void release();

public:
    static spWebImage get(const std::string& URL,
        std::function<void(spWebImage)> onSuccess,
        std::function<void(int,const std::string&)> onError);
private:
    friend class CachedImage;
    static std::unordered_map<std::string, std::shared_ptr<CachedImage> > _lookup;
    static std::deque<std::shared_ptr<CachedImage>> _loadQueue;
};