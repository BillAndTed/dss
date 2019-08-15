#pragma once

#include "oxygine-framework.h"
#include "EditorialData.h"
#include "EditorialMenu.h"

#include <deque>
#include <unordered_map>
#include <vector>

using namespace oxygine;

DECLARE_SMART(EditorialFeed, spEditorialFeed)
class EditorialFeed: public Actor
{
private:
    // pure data cache
    std::unordered_map< unsigned int, std::shared_ptr<EditorialData> >_parsedEditorialData;
    spEditorialMenu _menu;
    spTextField _instructions;
    spTextField _date;
private:
    // starting display at two days ago because today seems to lack data
    unsigned int _daysAgo = 2;
    bool _outstandingJSONRequest = false;
    bool _dirty = false;
public:

    virtual void onAdded2Stage() override;
    virtual void doUpdate(const UpdateState& us) override;

private:
    void fetchEditorialData(const unsigned int daysAgo);
    void parseEditorialData(const std::vector<unsigned char>& data);

    void setBackground();

    void showPreviousDay();
    void showNextDay();

private:
    static std::string generateJsonURL(const unsigned int daysAgo = 0);
};
