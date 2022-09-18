

enum EntityType {
	EntityType_Player,
	EntityType_Enemy,

	EntityType_PlayerBullet,
	EntityType_PlayerLaserCharge,
	EntityType_PlayerChargedLaserShot,

	EntityType_Count,
};

struct EntityInfo {
	int32 generation;
	int32 indexInBuffer;

	EntityType type;
};

struct EntityHandle {
	EntityType type;
	int32 id;
	int32 generation;
};

struct EntityTypeBuffer {
	int32 count;
	int32 entitySizeByte;
	int32 capacity;

	void* entities;
};

struct EntityManager {
	int32 entityCapacity;
	int32 nextID;
	EntityInfo* entities;

	EntityTypeBuffer buffers[EntityType_Count];
};


struct Entity {
	vec2 position;
	Sprite* sprite;

	bool toDelete = false;

	EntityHandle handle;
};

struct Player : Entity {
	real32 speed;
	vec2 size;
	bool chargingLaser;
	bool shootingLaser;
	int32 directionalVectors;

	EntityHandle currentLaserChargeHandle;
	EntityHandle currentChargedLaserShotHandle;
};

struct Enemy : Entity {
	vec2 speed;
	vec2 size;
};

struct PlayerBullet : Entity {
	vec2 speed;
	real32 lifetime;
	vec2 size;
};

struct PlayerLaserCharge : Entity {
	real32 growthRate;
	real32 strength;
	vec2 size;
};

struct PlayerChargedLaserShot : Entity {
	vec2 size;
	real32 speed;
	real32 power;
};

struct LevelManager {
	real32 timer;
	bool createNewEnemies;
};


struct MyData {
	EntityManager em;

	Sprite playerSprite;
	Sprite enemySprite;
	Sprite lemonSprite;
	Sprite playerBulletSprite;
	Sprite playerLaserChargeSprite;
	Sprite playerChargedLaserShotSprite;

	SoundClip soundDeepLaser;

	LevelManager lm;
};