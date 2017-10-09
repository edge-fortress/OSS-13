#include "Object.hpp"

#include "Shared/Math.hpp"

#include "Client.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/UI/UI.hpp"
#include "Graphics/UI/UIModule/GameProcessUI.hpp"
#include "Graphics/Sprite.hpp"
#include "Graphics/TileGrid.hpp"

Object::Object() :
    id(0), sprite(nullptr),
    direction(uf::Direction::NONE), dense(false),
    layer(0), shiftingSpeed(0),
    tile(nullptr)
{ }

void Object::SetSprite(uint id) {
	sprite = CC::Get()->RM.GetSprite(id);
}

void Object::SetDirection(const uf::Direction direction) {
    // cut the diagonal directions
	this->direction = uf::Direction(char(direction) % 4);
}

void Object::SetSpeed(float speed) {
	shiftingSpeed = speed;
}

void Object::SetShifting(uf::Direction direction, float speed) {
	uf::vec2i directionVect = uf::DirectionToVect(direction);
	shiftingDirection = directionVect;
	shiftingSpeed = speed;
}

void Object::ResetShifting() {
    shiftingDirection = {};
    shift = {};
	shiftingSpeed = 0;
}

void Object::Draw(sf::RenderTarget *target, uf::vec2i pos) {
    TileGrid *tileGrid = tile->GetTileGrid();
    if (!tileGrid) {
        CC::log << "Error: try to draw object from unplaced tiled!" << std::endl;
    }
    int tileSize = tileGrid->GetTileSize();
    if (sprite) sprite->Draw(target, pos + shift * tileSize, direction);
}

void Object::Update(sf::Time timeElapsed) {
    if (shiftingDirection) {
        shift += shiftingDirection * timeElapsed.asSeconds() * shiftingSpeed;
    }

    if (shift) {
        float delta = timeElapsed.asSeconds() * shiftingSpeed;
        if (!shiftingDirection.x) {
            if (shift.x * uf::sgn(shift.x) > delta)
                shift.x -= uf::sgn(shift.x) * delta;
            else shift.x = 0;
        }
        if (!shiftingDirection.y) {
            if (shift.y * uf::sgn(shift.y) > delta)
                shift.y -= uf::sgn(shift.y) * delta;
            else shift.y = 0;
        }
    }
}

void Object::ReverseShifting(uf::Direction direction) {
	uf::vec2i directionVect = uf::DirectionToVect(direction);
	if (directionVect.x) shiftingDirection.x = 0;
	if (directionVect.y) shiftingDirection.y = 0;
	shift -= directionVect;
}

uint Object::GetID() const { return id; }
std::string Object::GetName() const { return name; }
Sprite *Object::GetSprite() const { return sprite; }
uint Object::GetLayer() const { return layer; }
bool Object::PixelTransparent(uf::vec2i pixel) {
    return sprite->PixelTransparent(pixel);
}

Tile *Object::GetTile() { return tile; }
sf::Vector2f Object::GetShift() const { return shift; }
sf::Vector2i Object::GetShiftingDirection() const { return shiftingDirection; }
bool Object::IsDense() { return dense; }