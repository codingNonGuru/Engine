#include "Game/Types.hpp"

Word const TerrainModelBuffers::INDEX_LINKS = Word("IndexLinks");
Word const TerrainModelBuffers::POSITION_INPUT = Word("PositionInput");
Word const TerrainModelBuffers::POSITION_OUTPUT = Word("PositionOutput");
Word const TerrainModelBuffers::NORMAL_OUTPUT = Word("NormalOutput");
Word const TerrainModelBuffers::TANGENT_OUTPUT = Word("TangentOutput");
Word const TerrainModelBuffers::INDICES = Word("Indices");
int const TerrainModelBuffers::COUNT = 6;

Word const TerrainModelTextures::BASE_HEIGHT = Word("BaseHeight");
Word const TerrainModelTextures::DETAIL_HEIGHT = Word("DetailHeight");
int const TerrainModelTextures::COUNT = 2;

Word const SettlementModelBuffers::VERTEX_POSITIONS = Word("VertexPositions");
Word const SettlementModelBuffers::VERTEX_NORMALS = Word("VertexNormals");
Word const SettlementModelBuffers::INDICES = Word("Indices");
Word const SettlementModelBuffers::BUILDING_DATAS = Word("BuildingDatas");
Word const SettlementModelBuffers::BUILDING_INDICES = Word("BuildingIndices");
int const SettlementModelBuffers::COUNT = 5;

Word const Elements::MAIN_MENU = Word("MainMenu");
Word const Elements::NEW_GAME_MENU = Word("NewGameMenu");
Word const Elements::NEW_WORLD_MENU = Word("NewWorldMenu");
Word const Elements::WORLD_PREVIEW_PANEL = Word("WorldPreviewPanel");

Word const Cameras::PREGAME = Word("Pregame");
Word const Cameras::WORLD = Word("World");
