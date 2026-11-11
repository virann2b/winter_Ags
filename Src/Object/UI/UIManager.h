#pragma once
#include <vector>

#include "../Common/ActorBase/ActorBase.h"

class UIBase;

class UIManager : public ActorBase
{
public:

	UIManager(void);
	~UIManager(void) override = default;

	// “Ç‚İ‚İ
	void Load(void) override;

	/// <summary>
	/// UI‚ğ“o˜^‚·‚éi“o˜^‚µ‚½‡‚É•`‰æE‰ğ•ú‚ÍUIManager‚ªs‚¤j
	/// </summary>
	/// <param name="ui">new‚Å¶¬‚µ‚½UI</param>
	/// <returns>“o˜^‚µ‚½UI</returns>
	UIBase* AddUI(UIBase* ui);

private:

	// XV
	void SubUpdate(void) override;
	// •`‰æ
	void SubDraw(void) override;
	// ‰ğ•ú
	void SubRelease(void) override;

	// “o˜^‚³‚ê‚Ä‚¢‚éUIi•`‰æ‡j
	std::vector<UIBase*> uiList;
};

