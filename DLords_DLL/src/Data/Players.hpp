#pragma once

enum class EInvSlot : uint32_t
{
	eWeapons,
	eArmor,
	eJewerly,
	ePotionsScrolls,
	eKeysQuest,
	eJunkBag,
};

class CPlayer
{
private:
	bool m_isValid;
	
	std::string m_sName;

	dl_player_t* m_pData;

private:
	void SetBar(stat_bar_t* bar, int16_t value);
	void RestoreBar(stat_bar_t* bar, int16_t amount = 0);

	void Invalidate();

public:
	CPlayer();
	~CPlayer() = default;

	void Update(dl_player_t* m_pData);
	const bool IsValid() const;

	void SetHealth(int16_t value);
	void SetMana(int16_t value);

	void RestoreHealth(int16_t amount = 0);
	void RestoreMana(int16_t amount = 0);

	int16_t GetHealth();
	int16_t GetHealthMax();

	int16_t GetMana();
	int16_t GetManaMax();

	const std::string& GetName() const;

	dl_player_t* GetData();

	// Returns selected inventory page with all 20 slots in it.
	dl_player_inventory_t* GetInventory(EInvSlot eSlot);
	
	// Returns all items that may be on a player.
	dl_on_player_items_t* GetPlayerItems();

	// Returns item withing selected slot. (max 274)
	dl_item_t* GetOtherItems(size_t iSlot);

	bool IsHeraldyActive(int id);
	void SetHeraldy(int id, bool value);
};

extern CPlayer* GetPlayerData(int plyId);
extern CPlayer* GetLocalPlayer();