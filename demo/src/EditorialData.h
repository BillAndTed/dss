/*
EditorialData.h
Allow us to cache a subsection of editorial data so the Json doesn't have to hang around.
 */
#pragma once

#include <string> // for std::string
#include <memory> // for std::shared_ptr

#include "oxygine/json/json.h"

struct GameInfo
{
    std::string _headline;
    std::string _subHead;
    std::string _imgURL;
    unsigned int _imgWidth = 0;
    unsigned int _imgHeight = 0;
    std::string _date;
    std::string _id;

    static std::shared_ptr<GameInfo> parse(const Json::Value& game);
};

struct EditorialData
{
    std::string _copyright;
    std::vector< std::shared_ptr<GameInfo> > _games;

    static std::shared_ptr<EditorialData> parse(const Json::Value& root);
};