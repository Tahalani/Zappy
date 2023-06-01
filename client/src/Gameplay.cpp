/*
** EPITECH PROJECT, 2023
** Zappy
** File description:
** gameplay
*/

#include "Gameplay.hpp"

Gameplay::Gameplay(std::shared_ptr<Window> _window) : _window(_window)
{
    this->_map = Map(10, 10);
    this->_characters = std::vector<Character>();
    this->_currentCharacterId = 0;
    this->_currentCharacterIndex = 0;
}

void Gameplay::initPlayer(Vector3 pos)
{
    Character player(0, 0, pos);

    _characters.push_back(player);
}

void Gameplay::runPlayers(void)
{
    for (auto &character : this->_characters) {
        character.run();
    }
}

void Gameplay::run(void)
{
    this->_map.run();
    this->handleInput();
    this->runPlayers();
}

void Gameplay::setCurrentCharacter()
{
    if (this->_currentCharacterId >= this->_characters.size())
        this->_currentCharacterId = 0;
    for (std::size_t i = 0; i < this->_characters.size(); i++) {
        if (this->_currentCharacterId == i) {
            this->_currentCharacter = this->_characters[i];
            this->_currentCharacterId++;
            break;
        }
    }
}

void Gameplay::handleInput(void)
{
    if (IsKeyDown(KEY_ESCAPE))
        this->_window->setExit(true);
    if (IsKeyReleased(KEY_TAB)) {
        this->setCurrentCharacter();
        this->_window->setCamera({_currentCharacter.getPosition().x, _currentCharacter.getPosition().y + (float)1.5, _currentCharacter.getPosition().z - (float)0.5}, { 10.0f, 2.0f, 10.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE);
    }
    if (IsKeyDown(KEY_R)) {
        this->_window->setCamera({ -5.0f, 15.0f, 10.0f }, { 10.0f, 2.0f, 10.0f }, { 0.0f, 1.0f, 0.0f }, 100.0f, CAMERA_PERSPECTIVE);
    }  
}