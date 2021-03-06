/*
 * Copyright (c) 2010-2012 OTClient <https://github.com/edubart/otclient>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "otclient.h"
#include <framework/luascript/luainterface.h>
#include <otclient/luascript/luavaluecasts.h>
#include <otclient/core/game.h>
#include <otclient/core/tile.h>
#include <otclient/core/container.h>
#include <otclient/core/item.h>
#include <otclient/core/effect.h>
#include <otclient/core/missile.h>
#include <otclient/core/statictext.h>
#include <otclient/core/animatedtext.h>
#include <otclient/core/creature.h>
#include <otclient/core/player.h>
#include <otclient/core/localplayer.h>
#include <otclient/core/map.h>
#include <otclient/core/thingtypemanager.h>
#include <otclient/core/spritemanager.h>
#include <otclient/core/shadermanager.h>
#include <otclient/net/protocolgame.h>
#include <otclient/ui/uiitem.h>
#include <otclient/ui/uicreature.h>
#include <otclient/ui/uimap.h>
#include <otclient/ui/uiprogressrect.h>
#include <otclient/core/outfit.h>

void OTClient::registerLuaFunctions()
{
    g_lua.registerSingletonClass("g_things");
    g_lua.bindSingletonFunction("g_things", "loadDat", &ThingTypeManager::loadDat, &g_things);
    g_lua.bindSingletonFunction("g_things", "loadOtb", &ThingTypeManager::loadOtb, &g_things);
    g_lua.bindSingletonFunction("g_things", "loadXml", &ThingTypeManager::loadXml, &g_things);
    g_lua.bindSingletonFunction("g_things", "getDatSignature", &ThingTypeManager::getDatSignature, &g_things);

    g_lua.registerSingletonClass("g_sprites");
    g_lua.bindSingletonFunction("g_sprites", "loadSpr", &SpriteManager::loadSpr, &g_sprites);
    g_lua.bindSingletonFunction("g_sprites", "unload", &SpriteManager::unload, &g_sprites);
    g_lua.bindSingletonFunction("g_sprites", "isLoaded", &SpriteManager::isLoaded, &g_sprites);
    g_lua.bindSingletonFunction("g_sprites", "getSprSignature", &SpriteManager::getSignature, &g_sprites);
    g_lua.bindSingletonFunction("g_sprites", "getSpritesCount", &SpriteManager::getSpritesCount, &g_sprites);

    g_lua.registerSingletonClass("g_map");
    g_lua.bindSingletonFunction("g_map", "isLookPossible", &Map::isLookPossible, &g_map);
    g_lua.bindSingletonFunction("g_map", "isCovered", &Map::isCovered, &g_map);
    g_lua.bindSingletonFunction("g_map", "isCompletelyCovered", &Map::isCompletelyCovered, &g_map);
    g_lua.bindSingletonFunction("g_map", "addThing", &Map::addThing, &g_map);
    g_lua.bindSingletonFunction("g_map", "getThing", &Map::getThing, &g_map);
    g_lua.bindSingletonFunction("g_map", "removeThingByPos", &Map::removeThingByPos, &g_map);
    g_lua.bindSingletonFunction("g_map", "removeThing", &Map::removeThing, &g_map);
    g_lua.bindSingletonFunction("g_map", "cleanTile", &Map::cleanTile, &g_map);
    g_lua.bindSingletonFunction("g_map", "cleanTexts", &Map::cleanTexts, &g_map);
    g_lua.bindSingletonFunction("g_map", "getTile", &Map::getTile, &g_map);
    g_lua.bindSingletonFunction("g_map", "setCentralPosition", &Map::setCentralPosition, &g_map);
    g_lua.bindSingletonFunction("g_map", "getCentralPosition", &Map::getCentralPosition, &g_map);
    g_lua.bindSingletonFunction("g_map", "getCreatureById", &Map::getCreatureById, &g_map);
    g_lua.bindSingletonFunction("g_map", "removeCreatureById", &Map::removeCreatureById, &g_map);
    g_lua.bindSingletonFunction("g_map", "getSpectators", &Map::getSpectators, &g_map);
    g_lua.bindSingletonFunction("g_map", "findPath", &Map::findPath, &g_map);
    g_lua.bindSingletonFunction("g_map", "loadOtbm", &Map::loadOtbm, &g_map);
    //g_lua.bindSingletonFunction("g_map", "saveOtbm", &Map::saveOtbm, &g_map);
    g_lua.bindSingletonFunction("g_map", "loadOtcm", &Map::loadOtcm, &g_map);
    g_lua.bindSingletonFunction("g_map", "saveOtcm", &Map::saveOtcm, &g_map);

    g_lua.registerSingletonClass("g_game");
    g_lua.bindSingletonFunction("g_game", "loginWorld", &Game::loginWorld, &g_game);
    g_lua.bindSingletonFunction("g_game", "cancelLogin", &Game::cancelLogin, &g_game);
    g_lua.bindSingletonFunction("g_game", "forceLogout", &Game::forceLogout, &g_game);
    g_lua.bindSingletonFunction("g_game", "safeLogout", &Game::safeLogout, &g_game);
    g_lua.bindSingletonFunction("g_game", "walk", &Game::walk, &g_game);
    g_lua.bindSingletonFunction("g_game", "autoWalk", &Game::autoWalk, &g_game);
    g_lua.bindSingletonFunction("g_game", "forceWalk", &Game::forceWalk, &g_game);
    g_lua.bindSingletonFunction("g_game", "turn", &Game::turn, &g_game);
    g_lua.bindSingletonFunction("g_game", "stop", &Game::stop, &g_game);
    g_lua.bindSingletonFunction("g_game", "look", &Game::look, &g_game);
    g_lua.bindSingletonFunction("g_game", "move", &Game::move, &g_game);
    g_lua.bindSingletonFunction("g_game", "moveToParentContainer", &Game::moveToParentContainer, &g_game);
    g_lua.bindSingletonFunction("g_game", "rotate", &Game::rotate, &g_game);
    g_lua.bindSingletonFunction("g_game", "use", &Game::use, &g_game);
    g_lua.bindSingletonFunction("g_game", "useWith", &Game::useWith, &g_game);
    g_lua.bindSingletonFunction("g_game", "useInventoryItem", &Game::useInventoryItem, &g_game);
    g_lua.bindSingletonFunction("g_game", "useInventoryItemWith", &Game::useInventoryItemWith, &g_game);
    g_lua.bindSingletonFunction("g_game", "open", &Game::open, &g_game);
    g_lua.bindSingletonFunction("g_game", "openParent", &Game::openParent, &g_game);
    g_lua.bindSingletonFunction("g_game", "close", &Game::close, &g_game);
    g_lua.bindSingletonFunction("g_game", "refreshContainer", &Game::refreshContainer, &g_game);
    g_lua.bindSingletonFunction("g_game", "attack", &Game::attack, &g_game);
    g_lua.bindSingletonFunction("g_game", "cancelAttack", &Game::cancelAttack, &g_game);
    g_lua.bindSingletonFunction("g_game", "follow", &Game::follow, &g_game);
    g_lua.bindSingletonFunction("g_game", "cancelFollow", &Game::cancelFollow, &g_game);
    g_lua.bindSingletonFunction("g_game", "cancelAttackAndFollow", &Game::cancelAttackAndFollow, &g_game);
    g_lua.bindSingletonFunction("g_game", "talk", &Game::talk, &g_game);
    g_lua.bindSingletonFunction("g_game", "talkChannel", &Game::talkChannel, &g_game);
    g_lua.bindSingletonFunction("g_game", "talkPrivate", &Game::talkPrivate, &g_game);
    g_lua.bindSingletonFunction("g_game", "openPrivateChannel", &Game::openPrivateChannel, &g_game);
    g_lua.bindSingletonFunction("g_game", "requestChannels", &Game::requestChannels, &g_game);
    g_lua.bindSingletonFunction("g_game", "joinChannel", &Game::joinChannel, &g_game);
    g_lua.bindSingletonFunction("g_game", "leaveChannel", &Game::leaveChannel, &g_game);
    g_lua.bindSingletonFunction("g_game", "closeNpcChannel", &Game::closeNpcChannel, &g_game);
    g_lua.bindSingletonFunction("g_game", "openOwnChannel", &Game::openOwnChannel, &g_game);
    g_lua.bindSingletonFunction("g_game", "inviteToOwnChannel", &Game::inviteToOwnChannel, &g_game);
    g_lua.bindSingletonFunction("g_game", "excludeFromOwnChannel", &Game::excludeFromOwnChannel, &g_game);
    g_lua.bindSingletonFunction("g_game", "partyInvite", &Game::partyInvite, &g_game);
    g_lua.bindSingletonFunction("g_game", "partyJoin", &Game::partyJoin, &g_game);
    g_lua.bindSingletonFunction("g_game", "partyRevokeInvitation", &Game::partyRevokeInvitation, &g_game);
    g_lua.bindSingletonFunction("g_game", "partyPassLeadership", &Game::partyPassLeadership, &g_game);
    g_lua.bindSingletonFunction("g_game", "partyLeave", &Game::partyLeave, &g_game);
    g_lua.bindSingletonFunction("g_game", "partyShareExperience", &Game::partyShareExperience, &g_game);
    g_lua.bindSingletonFunction("g_game", "requestOutfit", &Game::requestOutfit, &g_game);
    g_lua.bindSingletonFunction("g_game", "changeOutfit", &Game::changeOutfit, &g_game);
    g_lua.bindSingletonFunction("g_game", "addVip", &Game::addVip, &g_game);
    g_lua.bindSingletonFunction("g_game", "removeVip", &Game::removeVip, &g_game);
    g_lua.bindSingletonFunction("g_game", "setChaseMode", &Game::setChaseMode, &g_game);
    g_lua.bindSingletonFunction("g_game", "setFightMode", &Game::setFightMode, &g_game);
    g_lua.bindSingletonFunction("g_game", "setSafeFight", &Game::setSafeFight, &g_game);
    g_lua.bindSingletonFunction("g_game", "getChaseMode", &Game::getChaseMode, &g_game);
    g_lua.bindSingletonFunction("g_game", "getFightMode", &Game::getFightMode, &g_game);
    g_lua.bindSingletonFunction("g_game", "isSafeFight", &Game::isSafeFight, &g_game);
    g_lua.bindSingletonFunction("g_game", "inspectNpcTrade", &Game::inspectNpcTrade, &g_game);
    g_lua.bindSingletonFunction("g_game", "buyItem", &Game::buyItem, &g_game);
    g_lua.bindSingletonFunction("g_game", "sellItem", &Game::sellItem, &g_game);
    g_lua.bindSingletonFunction("g_game", "closeNpcTrade", &Game::closeNpcTrade, &g_game);
    g_lua.bindSingletonFunction("g_game", "requestTrade", &Game::requestTrade, &g_game);
    g_lua.bindSingletonFunction("g_game", "inspectTrade", &Game::inspectTrade, &g_game);
    g_lua.bindSingletonFunction("g_game", "acceptTrade", &Game::acceptTrade, &g_game);
    g_lua.bindSingletonFunction("g_game", "rejectTrade", &Game::rejectTrade, &g_game);
    g_lua.bindSingletonFunction("g_game", "reportBug", &Game::reportBug, &g_game);
    g_lua.bindSingletonFunction("g_game", "reportRuleVilation", &Game::reportRuleVilation, &g_game);
    g_lua.bindSingletonFunction("g_game", "debugReport", &Game::debugReport, &g_game);
    g_lua.bindSingletonFunction("g_game", "editText", &Game::editText, &g_game);
    g_lua.bindSingletonFunction("g_game", "editList", &Game::editList, &g_game);
    g_lua.bindSingletonFunction("g_game", "requestQuestLog", &Game::requestQuestLog, &g_game);
    g_lua.bindSingletonFunction("g_game", "requestQuestLine", &Game::requestQuestLine, &g_game);
    g_lua.bindSingletonFunction("g_game", "equipItem", &Game::equipItem, &g_game);
    g_lua.bindSingletonFunction("g_game", "mount", &Game::mount, &g_game);
    g_lua.bindSingletonFunction("g_game", "canPerformGameAction", &Game::canPerformGameAction, &g_game);
    g_lua.bindSingletonFunction("g_game", "canReportBugs", &Game::canReportBugs, &g_game);
    g_lua.bindSingletonFunction("g_game", "checkBotProtection", &Game::checkBotProtection, &g_game);
    g_lua.bindSingletonFunction("g_game", "isOnline", &Game::isOnline, &g_game);
    g_lua.bindSingletonFunction("g_game", "isDead", &Game::isDead, &g_game);
    g_lua.bindSingletonFunction("g_game", "isAttacking", &Game::isAttacking, &g_game);
    g_lua.bindSingletonFunction("g_game", "isFollowing", &Game::isFollowing, &g_game);
    g_lua.bindSingletonFunction("g_game", "getContainer", &Game::getContainer, &g_game);
    g_lua.bindSingletonFunction("g_game", "getContainers", &Game::getContainers, &g_game);
    g_lua.bindSingletonFunction("g_game", "getVips", &Game::getVips, &g_game);
    g_lua.bindSingletonFunction("g_game", "getAttackingCreature", &Game::getAttackingCreature, &g_game);
    g_lua.bindSingletonFunction("g_game", "getFollowingCreature", &Game::getFollowingCreature, &g_game);
    g_lua.bindSingletonFunction("g_game", "getServerBeat", &Game::getServerBeat, &g_game);
    g_lua.bindSingletonFunction("g_game", "getLocalPlayer", &Game::getLocalPlayer, &g_game);
    g_lua.bindSingletonFunction("g_game", "getProtocolGame", &Game::getProtocolGame, &g_game);
    g_lua.bindSingletonFunction("g_game", "getProtocolVersion", &Game::getProtocolVersion, &g_game);
    g_lua.bindSingletonFunction("g_game", "getCharacterName", &Game::getCharacterName, &g_game);
    g_lua.bindSingletonFunction("g_game", "getWorldName", &Game::getWorldName, &g_game);
    g_lua.bindSingletonFunction("g_game", "getGMActions", &Game::getGMActions, &g_game);
    g_lua.bindSingletonFunction("g_game", "getClientVersion", &Game::getClientVersion, &g_game);
    g_lua.bindSingletonFunction("g_game", "getFeature", &Game::getFeature, &g_game);

    g_lua.registerSingletonClass("g_shaders");
    g_lua.bindSingletonFunction("g_shaders", "createShader", &ShaderManager::createShader, &g_shaders);
    g_lua.bindSingletonFunction("g_shaders", "createFragmentShader", &ShaderManager::createFragmentShader, &g_shaders);
    g_lua.bindSingletonFunction("g_shaders", "createFragmentShaderFromCode", &ShaderManager::createFragmentShaderFromCode, &g_shaders);
    g_lua.bindSingletonFunction("g_shaders", "createItemShader", &ShaderManager::createItemShader, &g_shaders);
    g_lua.bindSingletonFunction("g_shaders", "createMapShader", &ShaderManager::createMapShader, &g_shaders);
    g_lua.bindSingletonFunction("g_shaders", "getDefaultItemShader", &ShaderManager::getDefaultItemShader, &g_shaders);
    g_lua.bindSingletonFunction("g_shaders", "getDefaultMapShader", &ShaderManager::getDefaultMapShader, &g_shaders);
    g_lua.bindSingletonFunction("g_shaders", "getShader", &ShaderManager::getShader, &g_shaders);

    g_lua.bindGlobalFunction("getOufitColor", Outfit::getColor);

    g_lua.registerClass<ProtocolGame, Protocol>();
    g_lua.bindClassStaticFunction<ProtocolGame>("create", []{ return ProtocolGamePtr(new ProtocolGame); });
    g_lua.bindClassMemberFunction<ProtocolGame>("login", &ProtocolGame::login);
    g_lua.bindClassMemberFunction<ProtocolGame>("safeSend", &ProtocolGame::safeSend);
    g_lua.bindClassMemberFunction<ProtocolGame>("sendExtendedOpcode", &ProtocolGame::sendExtendedOpcode);
    g_lua.bindClassMemberFunction<ProtocolGame>("addPosition", &ProtocolGame::addPosition);
    g_lua.bindClassMemberFunction<ProtocolGame>("setMapDescription", &ProtocolGame::setMapDescription);
    g_lua.bindClassMemberFunction<ProtocolGame>("setFloorDescription", &ProtocolGame::setFloorDescription);
    g_lua.bindClassMemberFunction<ProtocolGame>("setTileDescription", &ProtocolGame::setTileDescription);
    g_lua.bindClassMemberFunction<ProtocolGame>("getOutfit", &ProtocolGame::getOutfit);
    g_lua.bindClassMemberFunction<ProtocolGame>("getThing", &ProtocolGame::getThing);
    g_lua.bindClassMemberFunction<ProtocolGame>("getCreature", &ProtocolGame::getCreature);
    g_lua.bindClassMemberFunction<ProtocolGame>("getItem", &ProtocolGame::getItem);
    g_lua.bindClassMemberFunction<ProtocolGame>("getPosition", &ProtocolGame::getPosition);

    g_lua.registerClass<Container>();
    g_lua.bindClassMemberFunction<Container>("getItem", &Container::getItem);
    g_lua.bindClassMemberFunction<Container>("getItems", &Container::getItems);
    g_lua.bindClassMemberFunction<Container>("getItemsCount", &Container::getItemsCount);
    g_lua.bindClassMemberFunction<Container>("getSlotPosition", &Container::getSlotPosition);
    g_lua.bindClassMemberFunction<Container>("getName", &Container::getName);
    g_lua.bindClassMemberFunction<Container>("getId", &Container::getId);
    g_lua.bindClassMemberFunction<Container>("getCapacity", &Container::getCapacity);
    g_lua.bindClassMemberFunction<Container>("getContainerItem", &Container::getContainerItem);
    g_lua.bindClassMemberFunction<Container>("hasParent", &Container::hasParent);
    g_lua.bindClassMemberFunction<Container>("isClosed", &Container::isClosed);

    g_lua.registerClass<Thing>();
    g_lua.bindClassMemberFunction<Thing>("setId", &Thing::setId);
    g_lua.bindClassMemberFunction<Thing>("setPosition", &Thing::setPosition);
    g_lua.bindClassMemberFunction<Thing>("getId", &Thing::getId);
    g_lua.bindClassMemberFunction<Thing>("getPosition", &Thing::getPosition);
    g_lua.bindClassMemberFunction<Thing>("getStackPriority", &Thing::getStackPriority);
    g_lua.bindClassMemberFunction<Thing>("getAnimationPhases", &Thing::getAnimationPhases);
    g_lua.bindClassMemberFunction<Thing>("asThing", &Thing::asThing);
    g_lua.bindClassMemberFunction<Thing>("asItem", &Thing::asItem);
    g_lua.bindClassMemberFunction<Thing>("asCreature", &Thing::asCreature);
    g_lua.bindClassMemberFunction<Thing>("asEffect", &Thing::asEffect);
    g_lua.bindClassMemberFunction<Thing>("asMissile", &Thing::asMissile);
    g_lua.bindClassMemberFunction<Thing>("asPlayer", &Thing::asPlayer);
    g_lua.bindClassMemberFunction<Thing>("asLocalPlayer", &Thing::asLocalPlayer);
    g_lua.bindClassMemberFunction<Thing>("asAnimatedText", &Thing::asAnimatedText);
    g_lua.bindClassMemberFunction<Thing>("asStaticText", &Thing::asStaticText);
    g_lua.bindClassMemberFunction<Thing>("isItem", &Thing::isItem);
    g_lua.bindClassMemberFunction<Thing>("isCreature", &Thing::isCreature);
    g_lua.bindClassMemberFunction<Thing>("isEffect", &Thing::isEffect);
    g_lua.bindClassMemberFunction<Thing>("isMissile", &Thing::isMissile);
    g_lua.bindClassMemberFunction<Thing>("isPlayer", &Thing::isPlayer);
    g_lua.bindClassMemberFunction<Thing>("isLocalPlayer", &Thing::isLocalPlayer);
    g_lua.bindClassMemberFunction<Thing>("isAnimatedText", &Thing::isAnimatedText);
    g_lua.bindClassMemberFunction<Thing>("isStaticText", &Thing::isStaticText);
    g_lua.bindClassMemberFunction<Thing>("isGround", &Thing::isGround);
    g_lua.bindClassMemberFunction<Thing>("isGroundBorder", &Thing::isGroundBorder);
    g_lua.bindClassMemberFunction<Thing>("isOnBottom", &Thing::isOnBottom);
    g_lua.bindClassMemberFunction<Thing>("isOnTop", &Thing::isOnTop);
    g_lua.bindClassMemberFunction<Thing>("isContainer", &Thing::isContainer);
    g_lua.bindClassMemberFunction<Thing>("isForceUse", &Thing::isForceUse);
    g_lua.bindClassMemberFunction<Thing>("isMultiUse", &Thing::isMultiUse);
    g_lua.bindClassMemberFunction<Thing>("isRotateable", &Thing::isRotateable);
    g_lua.bindClassMemberFunction<Thing>("isNotMoveable", &Thing::isNotMoveable);
    g_lua.bindClassMemberFunction<Thing>("isPickupable", &Thing::isPickupable);
    g_lua.bindClassMemberFunction<Thing>("isIgnoreLook", &Thing::isIgnoreLook);
    g_lua.bindClassMemberFunction<Thing>("isStackable", &Thing::isStackable);
    g_lua.bindClassMemberFunction<Thing>("isHookSouth", &Thing::isHookSouth);
    g_lua.bindClassMemberFunction<Thing>("isTranslucent", &Thing::isTranslucent);
    g_lua.bindClassMemberFunction<Thing>("isFullGround", &Thing::isFullGround);
    g_lua.bindClassMemberFunction<Thing>("getParentContainer", &Thing::getParentContainer);

    g_lua.registerClass<Creature, Thing>();
    g_lua.bindClassMemberFunction<Creature>("getId", &Creature::getId);
    g_lua.bindClassMemberFunction<Creature>("getName", &Creature::getName);
    g_lua.bindClassMemberFunction<Creature>("getHealthPercent", &Creature::getHealthPercent);
    g_lua.bindClassMemberFunction<Creature>("getSkull", &Creature::getSkull);
    g_lua.bindClassMemberFunction<Creature>("getShield", &Creature::getShield);
    g_lua.bindClassMemberFunction<Creature>("getEmblem", &Creature::getEmblem);
    g_lua.bindClassMemberFunction<Creature>("setOutfit", &Creature::setOutfit);
    g_lua.bindClassMemberFunction<Creature>("getOutfit", &Creature::getOutfit);
    g_lua.bindClassMemberFunction<Creature>("setSkullTexture", &Creature::setSkullTexture);
    g_lua.bindClassMemberFunction<Creature>("setShieldTexture", &Creature::setShieldTexture);
    g_lua.bindClassMemberFunction<Creature>("setEmblemTexture", &Creature::setEmblemTexture);
    g_lua.bindClassMemberFunction<Creature>("showStaticSquare", &Creature::showStaticSquare);
    g_lua.bindClassMemberFunction<Creature>("hideStaticSquare", &Creature::hideStaticSquare);
    g_lua.bindClassMemberFunction<Creature>("isWalking", &Creature::isWalking);
    g_lua.bindClassMemberFunction<Creature>("asMonster", &Creature::asMonster);
    g_lua.bindClassMemberFunction<Creature>("asNpc", &Creature::asNpc);

    g_lua.registerClass<Item, Thing>();
    g_lua.bindClassStaticFunction<Item>("create", &Item::create);
    g_lua.bindClassMemberFunction<Item>("setCount", &Item::setCount);
    g_lua.bindClassMemberFunction<Item>("getCount", &Item::getCount);
    g_lua.bindClassMemberFunction<Item>("getId", &Item::getId);
    g_lua.bindClassMemberFunction<Item>("isStackable", &Item::isStackable);

    g_lua.registerClass<Effect, Thing>();
    g_lua.registerClass<Missile, Thing>();
    g_lua.registerClass<StaticText, Thing>();
    g_lua.registerClass<AnimatedText, Thing>();

    g_lua.registerClass<Player, Creature>();
    g_lua.registerClass<Npc, Creature>();
    g_lua.registerClass<Monster, Creature>();

    g_lua.registerClass<LocalPlayer, Player>();
    g_lua.bindClassMemberFunction<LocalPlayer>("unlockWalk", &LocalPlayer::unlockWalk);
    g_lua.bindClassMemberFunction<LocalPlayer>("lockWalk", &LocalPlayer::lockWalk);
    g_lua.bindClassMemberFunction<LocalPlayer>("canWalk", &LocalPlayer::canWalk);
    g_lua.bindClassMemberFunction<LocalPlayer>("setStates", &LocalPlayer::setStates);
    g_lua.bindClassMemberFunction<LocalPlayer>("setSkill", &LocalPlayer::setSkill);
    g_lua.bindClassMemberFunction<LocalPlayer>("setHealth", &LocalPlayer::setHealth);
    g_lua.bindClassMemberFunction<LocalPlayer>("setFreeCapacity", &LocalPlayer::setFreeCapacity);
    g_lua.bindClassMemberFunction<LocalPlayer>("setExperience", &LocalPlayer::setExperience);
    g_lua.bindClassMemberFunction<LocalPlayer>("setLevel", &LocalPlayer::setLevel);
    g_lua.bindClassMemberFunction<LocalPlayer>("setMana", &LocalPlayer::setMana);
    g_lua.bindClassMemberFunction<LocalPlayer>("setMagicLevel", &LocalPlayer::setMagicLevel);
    g_lua.bindClassMemberFunction<LocalPlayer>("setSoul", &LocalPlayer::setSoul);
    g_lua.bindClassMemberFunction<LocalPlayer>("setStamina", &LocalPlayer::setStamina);
    g_lua.bindClassMemberFunction<LocalPlayer>("setKnown", &LocalPlayer::setKnown);
    g_lua.bindClassMemberFunction<LocalPlayer>("setInventoryItem", &LocalPlayer::setInventoryItem);
    g_lua.bindClassMemberFunction<LocalPlayer>("getStates", &LocalPlayer::getStates);
    g_lua.bindClassMemberFunction<LocalPlayer>("getSkillLevel", &LocalPlayer::getSkillLevel);
    g_lua.bindClassMemberFunction<LocalPlayer>("getSkillLevelPercent", &LocalPlayer::getSkillLevelPercent);
    g_lua.bindClassMemberFunction<LocalPlayer>("getHealth", &LocalPlayer::getHealth);
    g_lua.bindClassMemberFunction<LocalPlayer>("getMaxHealth", &LocalPlayer::getMaxHealth);
    g_lua.bindClassMemberFunction<LocalPlayer>("getFreeCapacity", &LocalPlayer::getFreeCapacity);
    g_lua.bindClassMemberFunction<LocalPlayer>("getExperience", &LocalPlayer::getExperience);
    g_lua.bindClassMemberFunction<LocalPlayer>("getLevel", &LocalPlayer::getLevel);
    g_lua.bindClassMemberFunction<LocalPlayer>("getLevelPercent", &LocalPlayer::getLevelPercent);
    g_lua.bindClassMemberFunction<LocalPlayer>("getMana", &LocalPlayer::getMana);
    g_lua.bindClassMemberFunction<LocalPlayer>("getMaxMana", &LocalPlayer::getMaxMana);
    g_lua.bindClassMemberFunction<LocalPlayer>("getMagicLevel", &LocalPlayer::getMagicLevel);
    g_lua.bindClassMemberFunction<LocalPlayer>("getMagicLevelPercent", &LocalPlayer::getMagicLevelPercent);
    g_lua.bindClassMemberFunction<LocalPlayer>("getSoul", &LocalPlayer::getSoul);
    g_lua.bindClassMemberFunction<LocalPlayer>("getStamina", &LocalPlayer::getStamina);
    g_lua.bindClassMemberFunction<LocalPlayer>("getInventoryItem", &LocalPlayer::getInventoryItem);
    g_lua.bindClassMemberFunction<LocalPlayer>("isKnown", &LocalPlayer::isKnown);
    g_lua.bindClassMemberFunction<LocalPlayer>("isPreWalking", &LocalPlayer::isPreWalking);
    g_lua.bindClassMemberFunction<LocalPlayer>("asLocalPlayer", &LocalPlayer::asLocalPlayer);

    g_lua.registerClass<Tile>();
    g_lua.bindClassMemberFunction<Tile>("clean", &Tile::clean);
    g_lua.bindClassMemberFunction<Tile>("addThing", &Tile::addThing);
    g_lua.bindClassMemberFunction<Tile>("getThing", &Tile::getThing);
    g_lua.bindClassMemberFunction<Tile>("getThingStackpos", &Tile::getThingStackpos);
    g_lua.bindClassMemberFunction<Tile>("getThingCount", &Tile::getThingCount);
    g_lua.bindClassMemberFunction<Tile>("getTopThing", &Tile::getTopThing);
    g_lua.bindClassMemberFunction<Tile>("removeThing", &Tile::removeThing);
    g_lua.bindClassMemberFunction<Tile>("getTopLookThing", &Tile::getTopLookThing);
    g_lua.bindClassMemberFunction<Tile>("getTopUseThing", &Tile::getTopUseThing);
    g_lua.bindClassMemberFunction<Tile>("getTopCreature", &Tile::getTopCreature);
    g_lua.bindClassMemberFunction<Tile>("getTopMoveThing", &Tile::getTopMoveThing);
    g_lua.bindClassMemberFunction<Tile>("getTopMultiUseThing", &Tile::getTopMultiUseThing);
    g_lua.bindClassMemberFunction<Tile>("getPosition", &Tile::getPosition);
    g_lua.bindClassMemberFunction<Tile>("getDrawElevation", &Tile::getDrawElevation);
    g_lua.bindClassMemberFunction<Tile>("getCreatures", &Tile::getCreatures);
    g_lua.bindClassMemberFunction<Tile>("getGround", &Tile::getGround);
    g_lua.bindClassMemberFunction<Tile>("isWalkable", &Tile::isWalkable);
    g_lua.bindClassMemberFunction<Tile>("isFullGround", &Tile::isFullGround);
    g_lua.bindClassMemberFunction<Tile>("isFullyOpaque", &Tile::isFullyOpaque);
    g_lua.bindClassMemberFunction<Tile>("isLookPossible", &Tile::isLookPossible);
    g_lua.bindClassMemberFunction<Tile>("hasCreature", &Tile::hasCreature);
    g_lua.bindClassMemberFunction<Tile>("isEmpty", &Tile::isEmpty);
    g_lua.bindClassMemberFunction<Tile>("isClickable", &Tile::isClickable);

    g_lua.registerClass<UIItem, UIWidget>();
    g_lua.bindClassStaticFunction<UIItem>("create", []{ return UIItemPtr(new UIItem); });
    g_lua.bindClassMemberFunction<UIItem>("setItemId", &UIItem::setItemId);
    g_lua.bindClassMemberFunction<UIItem>("setItemCount", &UIItem::setItemCount);
    g_lua.bindClassMemberFunction<UIItem>("setItemSubType", &UIItem::setItemSubType);
    g_lua.bindClassMemberFunction<UIItem>("setItem", &UIItem::setItem);
    g_lua.bindClassMemberFunction<UIItem>("setVirtual", &UIItem::setVirtual);
    g_lua.bindClassMemberFunction<UIItem>("clearItem", &UIItem::clearItem);
    g_lua.bindClassMemberFunction<UIItem>("getItemId", &UIItem::getItemId);
    g_lua.bindClassMemberFunction<UIItem>("getItemCount", &UIItem::getItemCount);
    g_lua.bindClassMemberFunction<UIItem>("getItemSubType", &UIItem::getItemSubType);
    g_lua.bindClassMemberFunction<UIItem>("getItem", &UIItem::getItem);
    g_lua.bindClassMemberFunction<UIItem>("isVirtual", &UIItem::isVirtual);

    g_lua.registerClass<UICreature, UIWidget>();
    g_lua.bindClassStaticFunction<UICreature>("create", []{ return UICreaturePtr(new UICreature); } );
    g_lua.bindClassMemberFunction<UICreature>("setCreature", &UICreature::setCreature);
    g_lua.bindClassMemberFunction<UICreature>("setFixedCreatureSize", &UICreature::setFixedCreatureSize);
    g_lua.bindClassMemberFunction<UICreature>("getCreature", &UICreature::getCreature);
    g_lua.bindClassMemberFunction<UICreature>("isFixedCreatureSize", &UICreature::isFixedCreatureSize);

    g_lua.registerClass<UIMap, UIWidget>();
    g_lua.bindClassStaticFunction<UIMap>("create", []{ return UIMapPtr(new UIMap); });
    g_lua.bindClassMemberFunction<UIMap>("drawSelf", &UIMap::drawSelf);
    g_lua.bindClassMemberFunction<UIMap>("setZoom", &UIMap::setZoom);
    g_lua.bindClassMemberFunction<UIMap>("zoomIn", &UIMap::zoomIn);
    g_lua.bindClassMemberFunction<UIMap>("zoomOut", &UIMap::zoomOut);
    g_lua.bindClassMemberFunction<UIMap>("followCreature", &UIMap::followCreature);
    g_lua.bindClassMemberFunction<UIMap>("setCameraPosition", &UIMap::setCameraPosition);
    g_lua.bindClassMemberFunction<UIMap>("setMaxZoomIn", &UIMap::setMaxZoomIn);
    g_lua.bindClassMemberFunction<UIMap>("setMaxZoomOut", &UIMap::setMaxZoomOut);
    g_lua.bindClassMemberFunction<UIMap>("setMultifloor", &UIMap::setMultifloor);
    g_lua.bindClassMemberFunction<UIMap>("setVisibleDimension", &UIMap::setVisibleDimension);
    g_lua.bindClassMemberFunction<UIMap>("setViewMode", &UIMap::setViewMode);
    g_lua.bindClassMemberFunction<UIMap>("setAutoViewMode", &UIMap::setAutoViewMode);
    g_lua.bindClassMemberFunction<UIMap>("setDrawFlags", &UIMap::setDrawFlags);
    g_lua.bindClassMemberFunction<UIMap>("setDrawTexts", &UIMap::setDrawTexts);
    g_lua.bindClassMemberFunction<UIMap>("setDrawMinimapColors", &UIMap::setDrawMinimapColors);
    g_lua.bindClassMemberFunction<UIMap>("setAnimated", &UIMap::setAnimated);
    g_lua.bindClassMemberFunction<UIMap>("setKeepAspectRatio", &UIMap::setKeepAspectRatio);
    g_lua.bindClassMemberFunction<UIMap>("setMapShader", &UIMap::setMapShader);
    g_lua.bindClassMemberFunction<UIMap>("isMultifloor", &UIMap::isMultifloor);
    g_lua.bindClassMemberFunction<UIMap>("isAutoViewModeEnabled", &UIMap::isAutoViewModeEnabled);
    g_lua.bindClassMemberFunction<UIMap>("isDrawingTexts", &UIMap::isDrawingTexts);
    g_lua.bindClassMemberFunction<UIMap>("isDrawingMinimapColors", &UIMap::isDrawingMinimapColors);
    g_lua.bindClassMemberFunction<UIMap>("isAnimating", &UIMap::isAnimating);
    g_lua.bindClassMemberFunction<UIMap>("isKeepAspectRatioEnabled", &UIMap::isKeepAspectRatioEnabled);
    g_lua.bindClassMemberFunction<UIMap>("getVisibleDimension", &UIMap::getVisibleDimension);
    g_lua.bindClassMemberFunction<UIMap>("getViewMode", &UIMap::getViewMode);
    g_lua.bindClassMemberFunction<UIMap>("getFollowingCreature", &UIMap::getFollowingCreature);
    g_lua.bindClassMemberFunction<UIMap>("getDrawFlags", &UIMap::getDrawFlags);
    g_lua.bindClassMemberFunction<UIMap>("getCameraPosition", &UIMap::getCameraPosition);
    g_lua.bindClassMemberFunction<UIMap>("getTile", &UIMap::getTile);
    g_lua.bindClassMemberFunction<UIMap>("getMaxZoomIn", &UIMap::getMaxZoomIn);
    g_lua.bindClassMemberFunction<UIMap>("getMaxZoomOut", &UIMap::getMaxZoomOut);
    g_lua.bindClassMemberFunction<UIMap>("getZoom", &UIMap::getZoom);
    g_lua.bindClassMemberFunction<UIMap>("getMapShader", &UIMap::getMapShader);

    g_lua.registerClass<UIProgressRect, UIWidget>();
    g_lua.bindClassStaticFunction<UIProgressRect>("create", []{ return UIProgressRectPtr(new UIProgressRect); } );
    g_lua.bindClassMemberFunction<UIProgressRect>("setPercent", &UIProgressRect::setPercent);
    g_lua.bindClassMemberFunction<UIProgressRect>("getPercent", &UIProgressRect::getPercent);
}
