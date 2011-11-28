/*
 * Copyright (c) 2010-2011 OTClient <https://github.com/edubart/otclient>
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

#include "creature.h"
#include "thingstype.h"
#include "localplayer.h"
#include "map.h"
#include "tile.h"
#include "item.h"

#include <framework/platform/platform.h>
#include <framework/graphics/graphics.h>
#include <framework/core/eventdispatcher.h>


Creature::Creature() : Thing()
{
    m_healthPercent = 0;
    m_showSquareColor = false;
    m_direction = Otc::South;

    m_walking = false;

    m_informationFont = g_fonts.getFont("verdana-11px-rounded");
}

void Creature::draw(const Point& p)
{
    const ThingType& type = getType();

    // TODO: activate on attack, follow, discover how 'attacked' works
    if(m_showSquareColor) {
        g_graphics.bindColor(Outfit::getColor(m_squareColor));
        g_graphics.drawBoundingRect(Rect(p + m_walkOffset - 8, Size(32, 32)), 2);
    }

    // Render creature
    for(m_yPattern = 0; m_yPattern < type.dimensions[ThingType::PatternY]; m_yPattern++) {

        // continue if we dont have this addon.
        if(m_yPattern > 0 && !(m_outfit.getAddons() & (1 << (m_yPattern-1))))
            continue;

        // draw white item
        g_graphics.bindColor(Fw::white);
        internalDraw(p + m_walkOffset, 0);

        // draw mask if exists
        if(type.dimensions[ThingType::Layers] > 1) {
            // switch to blend color mode
            g_graphics.bindBlendFunc(Fw::BlendColorzing);

            // head
            g_graphics.bindColor(m_outfit.getHeadColor());
            internalDraw(p + m_walkOffset, 1, Otc::SpriteYellowMask);

            // body
            g_graphics.bindColor(m_outfit.getBodyColor());
            internalDraw(p + m_walkOffset, 1, Otc::SpriteRedMask);

            // legs
            g_graphics.bindColor(m_outfit.getLegsColor());
            internalDraw(p + m_walkOffset, 1, Otc::SpriteGreenMask);

            // feet
            g_graphics.bindColor(m_outfit.getFeetColor());
            internalDraw(p + m_walkOffset, 1, Otc::SpriteBlueMask);

            // restore default blend func
            g_graphics.bindBlendFunc(Fw::BlendDefault);
            g_graphics.bindColor(Fw::white);
        }
    }
}

void Creature::drawInformation(int x, int y, bool useGray, const Rect& rect)
{
    Color fillColor = Color(96, 96, 96);

    if(!useGray)
        fillColor = m_informationColor;

    // calculate main rects
    Rect backgroundRect = Rect(x-(13.5), y, 27, 4);
    backgroundRect.bound(rect);

    Rect textRect = Rect(x - m_nameSize.width() / 2.0, y-15, m_nameSize);
    textRect.bound(rect);

    // distance them
    if(textRect.top() == rect.top())
        backgroundRect.moveTop(textRect.top() + 15);
    if(backgroundRect.bottom() == rect.bottom())
        textRect.moveTop(backgroundRect.top() - 15);

    // health rect is based on background rect, so no worries
    Rect healthRect = backgroundRect.expanded(-1);
    healthRect.setWidth((m_healthPercent / 100.0) * 25);

    // draw
    g_graphics.bindColor(Fw::black);
    g_graphics.drawFilledRect(backgroundRect);

    g_graphics.bindColor(fillColor);
    g_graphics.drawFilledRect(healthRect);

    m_informationFont->renderText(m_name, textRect, Fw::AlignTopCenter, fillColor);
}

void Creature::walk(const Position& position, bool inverse)
{
    // set walking state
    bool sameWalk = m_walking && !m_inverseWalking && inverse;
    m_walking = true;
    m_inverseWalking = inverse;
    int walkTimeFactor = 1;

    // set new direction
    if(m_position + Position(0, -1, 0) == position) {
        setDirection(Otc::North);
        m_walkOffset.y = 32;
    }
    else if(m_position + Position(1, 0, 0) == position) {
        setDirection(Otc::East);
        m_walkOffset.x = -32;
    }
    else if(m_position + Position(0, 1, 0) == position) {
        setDirection(Otc::South);
        m_walkOffset.y = -32;
    }
    else if(m_position + Position(-1, 0, 0) == position) {
        setDirection(Otc::West);
        m_walkOffset.x = 32;
    }
    else if(m_position + Position(1, -1, 0) == position) {
        setDirection(Otc::NorthEast);
        m_walkOffset.x = -32;
        m_walkOffset.y = 32;
        walkTimeFactor = 2;
    }
    else if(m_position + Position(1, 1, 0) == position) {
        setDirection(Otc::SouthEast);
        m_walkOffset.x = -32;
        m_walkOffset.y = -32;
        walkTimeFactor = 2;
    }
    else if(m_position + Position(-1, 1, 0) == position) {
        setDirection(Otc::SouthWest);
        m_walkOffset.x = 32;
        m_walkOffset.y = -32;
        walkTimeFactor = 2;
    }
    else if(m_position + Position(-1, -1, 0) == position) {
        setDirection(Otc::NorthWest);
        m_walkOffset.x = 32;
        m_walkOffset.y = 32;
        walkTimeFactor = 2;
    }
    else { // Teleport
        // we teleported, dont walk or change direction
        m_walking = false;
        m_walkOffset.x = 0;
        m_walkOffset.y = 0;
        m_animation = 0;
    }

    if(!m_inverseWalking) {
        m_walkOffset.x = 0;
        m_walkOffset.y = 0;
    }

    if(m_walking) {
        // get walk speed
        int groundSpeed = 100;

        ItemPtr ground = g_map.getTile(position)->getGround();
        if(ground)
            groundSpeed = ground->getType().parameters[ThingType::GroundSpeed];

        float walkTime = walkTimeFactor * 1000.0 * (float)groundSpeed / m_speed;
        walkTime = (walkTime == 0) ? 1000 : walkTime;

        m_walkTimePerPixel = walkTime / 32.0;
        if(!sameWalk)
            m_walkStartTicks = g_platform.getTicks();
        updateWalk();
    }
}

void Creature::updateWalk()
{
    const ThingType& type = getType();
    if(m_walking) {
        int elapsedTicks = g_platform.getTicks() - m_walkStartTicks;
        int totalPixelsWalked = std::min((int)round(elapsedTicks / m_walkTimePerPixel), 32);

        if(m_inverseWalking) {
            if(m_direction == Otc::North || m_direction == Otc::NorthEast || m_direction == Otc::NorthWest)
                m_walkOffset.y = 32 - totalPixelsWalked;
            else if(m_direction == Otc::South || m_direction == Otc::SouthEast || m_direction == Otc::SouthWest)
                m_walkOffset.y = totalPixelsWalked - 32;

            if(m_direction == Otc::East || m_direction == Otc::NorthEast || m_direction == Otc::SouthEast)
                m_walkOffset.x = totalPixelsWalked - 32;
            else if(m_direction == Otc::West || m_direction == Otc::NorthWest || m_direction == Otc::SouthWest)
                m_walkOffset.x = 32 - totalPixelsWalked;

            if(m_walkOffset.x == 0 && m_walkOffset.y == 0)
                cancelWalk(m_direction);
        }
        else {
            if(m_direction == Otc::North || m_direction == Otc::NorthEast || m_direction == Otc::NorthWest)
                m_walkOffset.y = -totalPixelsWalked;
            else if(m_direction == Otc::South || m_direction == Otc::SouthEast || m_direction == Otc::SouthWest)
                m_walkOffset.y = totalPixelsWalked;

            if(m_direction == Otc::East || m_direction == Otc::NorthEast || m_direction == Otc::SouthEast)
                m_walkOffset.x = totalPixelsWalked;
            else if(m_direction == Otc::West || m_direction == Otc::NorthWest || m_direction == Otc::SouthWest)
                m_walkOffset.x = -totalPixelsWalked;
        }

        int totalWalkTileTicks = (int)m_walkTimePerPixel*32 * 0.5;
        m_animation = (g_platform.getTicks() % totalWalkTileTicks) / (totalWalkTileTicks / (type.dimensions[ThingType::AnimationPhases] - 1)) + 1;
        g_dispatcher.scheduleEvent(std::bind(&Creature::updateWalk, asCreature()), m_walkTimePerPixel);

        if(totalPixelsWalked == 32)
            cancelWalk(m_direction);
    }
}

void Creature::cancelWalk(Otc::Direction direction)
{
    m_walking = false;
    m_walkStartTicks = 0;
    m_walkOffset.x = 0;
    m_walkOffset.y = 0;
    m_direction = direction;

    auto self = asCreature();
    g_dispatcher.scheduleEvent([=]() {
        if(!self->m_walking)
            self->m_animation = 0;
    }, m_walkTimePerPixel * 2);
}

void Creature::setName(const std::string& name)
{
    m_nameSize = m_informationFont->calculateTextRectSize(name);
    m_name = name;
}

void Creature::setHealthPercent(uint8 healthPercent)
{
    m_informationColor = Fw::black;

    if(healthPercent > 92) {
        m_informationColor.setGreen(188);
    }
    else if(healthPercent > 60) {
        m_informationColor.setRed(80);
        m_informationColor.setGreen(161);
        m_informationColor.setBlue(80);
    }
    else if(healthPercent > 30) {
        m_informationColor.setRed(161);
        m_informationColor.setGreen(161);
    }
    else if(healthPercent > 8) {
        m_informationColor.setRed(160);
        m_informationColor.setGreen(39);
        m_informationColor.setBlue(39);
    }
    else if(healthPercent > 3) {
        m_informationColor.setRed(160);
    }
    else {
        m_informationColor.setRed(79);
    }

    m_healthPercent = healthPercent;
}

void Creature::setDirection(Otc::Direction direction)
{
    if(direction >= 4) {
        if(direction == Otc::NorthEast || direction == Otc::SouthEast)
            m_xPattern = Otc::East;
        else if(direction == Otc::NorthWest || direction == Otc::SouthWest)
            m_xPattern = Otc::West;
    }
    else {
        m_xPattern = direction;
    }

    m_direction = direction;
}

const ThingType& Creature::getType()
{
    return g_thingsType.getThingType(m_outfit.getType(), ThingsType::Creature);
}
