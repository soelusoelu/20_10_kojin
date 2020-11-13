#include "SaveThis.h"
#include "../../Utility/LevelLoader.h"

SaveThis::SaveThis(GameObject& gameObject)
    : Component(gameObject)
{
}

SaveThis::~SaveThis() = default;

void SaveThis::finalize() {
    LevelLoader::saveGameObject(gameObject());
}
