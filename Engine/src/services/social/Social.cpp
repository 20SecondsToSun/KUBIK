#include "services/social/Social.h"

using namespace kubik;

VkontakteRef Social::vkontakte;
FacebookRef Social::facebook;
TwitterRef Social::twitter;

SocShare::SocShare() :connected(false), activity(ActivityID::PHOTO_STATUS)
{

}