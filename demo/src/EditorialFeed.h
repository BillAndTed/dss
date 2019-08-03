#pragma once

#include "oxygine-framework.h"
#include "EditorialData.h"
#include "EditorialMenu.h"

#include <deque>

using namespace oxygine;

DECLARE_SMART(EditorialFeed, spEditorialFeed)
class EditorialFeed: public Actor
{
private:
    // pure data cache
    std::shared_ptr<EditorialData> _parsedEditorialData;
    spEditorialMenu _menu;

public:

    virtual void onAdded2Stage() override;
    virtual void doUpdate(const UpdateState& us) override;

    void fetchEditorialData();
    void parseEditorialData(const std::string& data);

    void setBackground();

private:
    static std::string generateJsonURL(const unsigned int daysAgo = 0);
};
