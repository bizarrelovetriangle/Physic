#pragma once
#include "vector2.h"
#include "physic_object.h"
#include <vector>

class box_block : public physic_object {
public:
	box_block(vector2 pos = {}, vector2 size = { 100., 100. });
	void draw(sf::RenderWindow& window) override;
};