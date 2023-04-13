// WISHLIST:
// (1) implement generate random decks with all categories
// (2) implement triple draft
// (3) copy deck directly from program using link
// (4) sort by "natural order" in various places (and use green for win cons in mega draft)

#include "general/file.h"
#include "general/vectorUtility.h"
#include "general/parseArguments.h"
#include "general/randomUtility.h"
#include "general/ansi_codes.h"
#include "general/stringUtility.h"
#include "general/mapUtility.h"
#include <set>

RandUtil randUtil;

typedef const vector<string> CardList;

CardList SMALL_SPELLS = {"Tornado", "Zap", "Rage", "Giant Snowball", "Barbarian Barrel", "Earthquake", "The Log", "Arrows", "Royal Delivery"};
CardList LARGE_SPELLS = {"Freeze", "Fireball", "Poison", "Lightning", "Rocket"};
CardList NO_DMG_SPELLS = {"Goblin Barrel", "Graveyard", "Mirror", "Clone"};

CardList DMG_BUILDINGS = {"Cannon", "Tesla", "Bomb Tower", "Inferno Tower", "Mortar", "X-Bow"};
CardList SPAWNER_BUILDINGS = {"Goblin Hut", "Goblin Drill", "Goblin Cage", "Tombstone", "Furnace", "Barbarian Hut", "Elixir Collector"};

CardList SWARMS = {"Skeletons", "Skeleton Army", "Bats", "Spear Goblins", "Goblins", "Goblin Gang", "Fire Spirit", "Ice Spirit", "Electro Spirit", "Minions", "Minion Horde", "Heal Spirit", "Dart Goblin", "Princess", "Archers", "Firecracker", "Guards", "Wall Breakers", "Bomber"};
CardList L_GLASS_CANNONS = {"Zappies", "Skeleton Dragons", "Skeleton Barrel", "Magic Archer", "Mother Witch", "Flying Machine", "Barbarians", "Ice Wizard", "Electro Wizard", "Wizard", "Musketeer", "Three Musketeers"};
CardList U_GLASS_CANNONS = {"Royal Recruits", "Cannon Cart", "Mega Minion", "Royal Hogs", "Witch", "Hunter", "Fisherman", "Bandit", "Night Witch", "Battle Ram", "Electro Dragon", "Archer Queen", "Phoenix"};
CardList L_MINI_TANKS = {"Baby Dragon", "Ice Golem", "Miner", "Royal Ghost", "Executioner", "Lumberjack", "Inferno Dragon", "Elite Barbarians", "Mini P.E.K.K.A", "Dark Prince", "Sparky"};
CardList U_MINI_TANKS = {"Elixir Golem", "Balloon", "Hog Rider", "Battle Healer", "Knight", "Ram Rider", "Golden Knight", "Valkyrie", "Prince"};
CardList TANKS = {"Rascals", "Monk", "Bowler", "Skeleton King", "Mighty Miner", "Royal Giant", "Goblin Giant", "Giant Skeleton", "P.E.K.K.A", "Lava Hound", "Electro Giant", "Mega Knight", "Giant", "Golem"};

CardList AIR_TROOPS = {"Bats", "Minions", "Minion Horde", "Skeleton Dragons", "Skeleton Barrel", "Flying Machine", "Mega Minion", "Electro Dragon", "Phoenix", "Baby Dragon", "Inferno Dragon", "Balloon", "Lava Hound"};
CardList TOWER_CHASERS = {"Wall Breakers", "Skeleton Barrel", "Royal Hogs", "Battle Ram", "Ice Golem", "Elixir Golem", "Balloon", "Hog Rider", "Ram Rider", "Royal Giant", "Goblin Giant", "Lava Hound", "Electro Giant", "Giant", "Golem"};
CardList CHAMPIONS = {"Archer Queen", "Golden Knight", "Monk", "Skeleton King", "Mighty Miner"};
CardList GOBLINS = {"Goblin Barrel", "Goblin Cage", "Goblin Hut", "Goblin Drill", "Spear Goblins", "Goblins", "Goblin Gang", "Dart Goblin", "Goblin Giant"};
CardList BARBARIANS = {"Barbarian Barrel", "Barbarian Hut", "Barbarians", "Battle Ram", "Elite Barbarians"};
CardList ARCHERS = {"Arrows", "X-Bow", "Princess", "Archers", "Magic Archer", "Archer Queen"};
CardList ICE = {"Freeze", "Giant Snowball", "Ice Spirit", "Ice Wizard", "Ice Golem"};
CardList DRAGONS = {"Skeleton Dragons", "Electro Dragon", "Baby Dragon", "Inferno Dragon"};
CardList MAGIC = {"Fire Spirit", "Ice Spirit", "Electro Spirit", "Heal Spirit", "Ice Wizard", "Electro Wizard", "Wizard", "Mother Witch", "Witch", "Night Witch"};

CardList TARGET_AIR = {"Freeze", "Tornado", "Rage", "Zap", "Giant Snowball", "Arrows", "Royal Delivery", "Fireball", "Poison", "Lightning", "Rocket", "Tesla", "Inferno Tower", "Goblin Hut", "Furnace", "Bats", "Spear Goblins", "Fire Spirit", "Ice Spirit", "Electro Spirit", "Minions", "Minion Horde", "Heal Spirit", "Dart Goblin", "Princess", "Archers", "Firecracker", "Zappies", "Skeleton Dragons", "Magic Archer", "Mother Witch", "Flying Machine", "Ice Wizard", "Electro Wizard", "Wizard", "Musketeer", "Three Musketeers", "Mega Minion", "Witch", "Hunter", "Electro Dragon", "Archer Queen", "Phoenix", "Baby Dragon", "Executioner", "Inferno Dragon"};
CardList TARGET_AIR_PLUS = {"Goblin Gang", "Rascals", "Night Witch", "Lava Hound", "Ram Rider", "Goblin Giant", "Electro Giant"};
CardList WIN_CONDITIONS = {"Wall Breakers", "Skeleton Barrel", "Royal Hogs", "Battle Ram", "Elixir Golem", "Balloon", "Hog Rider", "Ram Rider", "Royal Giant", "Goblin Giant", "Lava Hound", "Electro Giant", "Giant", "Golem", "Goblin Barrel", "Graveyard", "Miner", "Goblin Drill", "X-Bow", "Mortar", "Three Musketeers", "Sparky", "Rocket"};
CardList WIN_CONDITIONS_PLUS = {"Mega Knight", "P.E.K.K.A", "Giant Skeleton", "Skeleton King", "Mighty Miner", "Golden Knight", "Mother Witch", "Magic Archer", "Firecracker", "Princess"};
CardList GROUP_OF_TROOPS = {"Skeletons", "Skeleton Army", "Bats", "Spear Goblins", "Goblins", "Goblin Gang", "Minions", "Minion Horde", "Archers", "Guards", "Wall Breakers", "Zappies", "Skeleton Dragons", "Barbarians", "Three Musketeers", "Royal Recruits", "Royal Hogs", "Elite Barbarians", "Rascals"};
CardList GROUP_OF_TROOPS_PLUS = {"Goblin Barrel", "Graveyard", "Tombstone", "Goblin Hut", "Furnace", "Barbarian Hut", "Goblin Drill", "Skeleton Barrel", "Mother Witch", "Witch", "Night Witch", "Battle Ram", "Elixir Golem", "Skeleton King", "Goblin Giant", "Lava Hound", "Golem"};
CardList SPLASH = {"Freeze", "Tornado", "Zap", "Rage", "Giant Snowball", "Barbarian Barrel", "Earthquake", "The Log", "Arrows", "Royal Delivery", "Fireball", "Poison", "Rocket", "Bomb Tower", "Mortar", "Furnace", "Fire Spirit", "Ice Spirit", "Heal Spirit", "Princess", "Firecracker", "Bomber", "Skeleton Dragons", "Magic Archer", "Ice Wizard", "Wizard", "Witch", "Baby Dragon", "Royal Ghost", "Executioner", "Dark Prince", "Sparky", "Valkyrie", "Bowler", "Skeleton King", "Mega Knight"};
CardList SPLASH_PLUS = {"Golden Knight", "Electro Spirit", "Electro Dragon", "Lightning", "Hunter", "Electro Wizard", "Goblin Drill", "Skeleton Barrel", "Phoenix", "Ice Golem", "Lumberjack", "Balloon", "Giant Skeleton", "Golem", "Wall Breakers", "Monk", "Mighty Miner"};
CardList SKELETONS = {"Skeletons", "Skeleton Army", "Skeleton Barrel", "Skeleton King", "Skeleton Dragons", "Giant Skeleton", "Tombstone", "Graveyard", "Witch"};
CardList SKELETONS_PLUS = {"Bomber", "Bomb Tower", "Wall Breakers", "Balloon", "Guards"};
CardList FIRE = {"Fireball", "Fire Spirit", "Furnace", "Inferno Tower", "Inferno Dragon", "Lava Hound", "Baby Dragon", "Skeleton Dragons", "Wizard"};
CardList FIRE_PLUS = {"Rocket", "Balloon", "Phoenix", "Firecracker", "Musketeer", "Three Musketeers"};
CardList ELECTRO = {"Electro Wizard", "Zappies", "Electro Dragon", "Electro Spirit", "Electro Giant", "Zap", "Lightning"};
CardList ELECTRO_PLUS = {"Tesla", "Sparky"};
CardList ROYAL = {"Royal Delivery", "Royal Recruits", "Royal Hogs", "Royal Ghost", "Royal Giant", "Princess", "Dark Prince", "Prince"};
CardList ROYAL_PLUS = {"Archer Queen", "Skeleton King", "Guards", "Knight", "Golden Knight", "Mega Knight"};
CardList SPAWNERS = {"Tombstone", "Furnace", "Goblin Hut", "Barbarian Hut", "Goblin Drill", "Witch", "Night Witch", "Mother Witch", "Graveyard", "Skeleton King"};
CardList SPAWNERS_PLUS = {"Barbarian Barrel", "Royal Delivery", "Goblin Barrel", "Goblin Cage", "Skeleton Barrel", "Battle Ram", "Cannon Cart", "Phoenix", "Elixir Golem", "Goblin Giant", "Lava Hound", "Golem"};
CardList HAS_DEPENDENTS = {"Electro Wizard", "Lumberjack"}; // and a lot more...

map<string, vector<string>> CATEGORY_MAP, CAT_PLUS_MAP;

void initializeCategoryMap() {

	CATEGORY_MAP = mapUtil::makeMapFromVectorOfPairs(vector<pair<string, vector<string>>>({
		{"small spells", SMALL_SPELLS},
		{"large spells", LARGE_SPELLS},
		{"non-damaging spells", NO_DMG_SPELLS},
		{"spells", {}},
		{"damaging buildings", DMG_BUILDINGS},
		{"spawner buildings", SPAWNER_BUILDINGS},
		{"buildings", {}},
		{"swarms", SWARMS},
		{"lower glass cannons", L_GLASS_CANNONS},
		{"upper glass cannons", U_GLASS_CANNONS},
		{"glass cannons", {}},
		{"lower mini-tanks", L_MINI_TANKS},
		{"upper mini-tanks", U_MINI_TANKS},
		{"mini-tanks", {}},
		{"tanks", TANKS},
		{"troops", {}},
		{"all", {}},
		{"air troops", AIR_TROOPS},
		{"tower chasers", TOWER_CHASERS},
		{"champions", CHAMPIONS},
		{"goblins", GOBLINS},
		{"barbarians", BARBARIANS},
		{"archers", ARCHERS},
		{"ice", ICE},
		{"dragons", DRAGONS},
		{"magic", MAGIC},
		{"target air", TARGET_AIR},
		{"win conditions", WIN_CONDITIONS},
		{"group of troops", GROUP_OF_TROOPS},
		{"splash", SPLASH},
		{"skeletons", SKELETONS},
		{"fire", FIRE},
		{"electro", ELECTRO},
		{"royal", ROYAL},
		{"spawners", SPAWNERS},
		{"has dependents", HAS_DEPENDENTS}
	}));

	CAT_PLUS_MAP = mapUtil::makeMapFromVectorOfPairs(vector<pair<string, vector<string>>>({
		{"target air", TARGET_AIR_PLUS},
		{"win conditions", WIN_CONDITIONS_PLUS},
		{"group of troops", GROUP_OF_TROOPS_PLUS},
		{"splash", SPLASH_PLUS},
		{"skeletons", SKELETONS_PLUS},
		{"fire", FIRE_PLUS},
		{"electro", ELECTRO_PLUS},
		{"royal", ROYAL_PLUS},
		{"spawners", SPAWNERS_PLUS}
	}));
	
	CATEGORY_MAP.at("spells") = vecUtil::concatenate(vector<vector<string>>({SMALL_SPELLS, LARGE_SPELLS, NO_DMG_SPELLS}));
	CATEGORY_MAP.at("buildings") = vecUtil::concatenate(vector<vector<string>>({DMG_BUILDINGS, SPAWNER_BUILDINGS}));
	CATEGORY_MAP.at("glass cannons") = vecUtil::concatenate(vector<vector<string>>({L_GLASS_CANNONS, U_GLASS_CANNONS}));
	CATEGORY_MAP.at("mini-tanks") = vecUtil::concatenate(vector<vector<string>>({L_MINI_TANKS, U_MINI_TANKS}));
	
	CATEGORY_MAP.at("troops") = vecUtil::concatenate(vector<vector<string>>{
		SWARMS, CATEGORY_MAP.at("glass cannons"), CATEGORY_MAP.at("mini-tanks"), TANKS
	});

	CATEGORY_MAP.at("all") = vecUtil::concatenate(vector<vector<string>>{
		CATEGORY_MAP.at("spells"), CATEGORY_MAP.at("troops"), CATEGORY_MAP.at("buildings")
	});

	for (string plusable : mapUtil::getKeys(CAT_PLUS_MAP)) {
		CATEGORY_MAP.insert({
			plusable + "+",
			vecUtil::concatenate(vector<vector<string>>({CATEGORY_MAP.at(plusable), CAT_PLUS_MAP.at(plusable)}))
		});
	}

	for (string card : CATEGORY_MAP.at("spawners+")) {
		CATEGORY_MAP.at("has dependents").push_back(card);
	}

	CATEGORY_MAP.insert({"has dependents+", vecUtil::concatenate(vector<vector<string>>{
		CATEGORY_MAP.at("has dependents"), CATEGORY_MAP.at("group of troops")
	})});
	
}

vector<string> read() {
	
	vector<string> raw, cards, categories, removals, categoryRemovals, forces, categoryForces;
	file::inputStrVecFrom(raw, "internal_use.txt");

	for (string line : raw) {
		if (strUtil::beginsWith(line, "[")) {
			categories.push_back(line.substr(1, line.size() - 2));
		} else if (strUtil::beginsWith(line, "#") || line.empty()) {
			// it's a comment or an empty line, ignore it
		} else if (strUtil::beginsWith(line, "-[")) {
			categoryRemovals.push_back(line.substr(2, line.size() - 3));
		} else if (strUtil::beginsWith(line, "-")) {
			removals.push_back(line.substr(1));
		} else if (strUtil::beginsWith(line, "*[")) {
			categoryForces.push_back(line.substr(2, line.size() - 3));
			categories.push_back(line.substr(2, line.size() - 3));
		} else if (strUtil::beginsWith(line, "*")) {
			forces.push_back(line.substr(1));
			cards.push_back(line.substr(1));
		} else {
			cards.push_back(line);
		}
	}

	for (string cat : categories) {
		cards.insert(cards.end(), CATEGORY_MAP.at(cat).begin(), CATEGORY_MAP.at(cat).end());
	}

	for (string catForce : categoryForces) {
		forces.insert(forces.end(), CATEGORY_MAP.at(catForce).begin(), CATEGORY_MAP.at(catForce).end());
	}

	for (string rem : removals) {
		if (!vecUtil::contains(forces, rem)) {
			vecUtil::removeAllInstances(cards, rem);
		}
	}

	for (string cat : categoryRemovals) {
		for (string card : CATEGORY_MAP.at(cat)) {
			if (!vecUtil::contains(forces, card)) {
				vecUtil::removeAllInstances(cards, card);
			}
		}
	}

	set<string> noDup(cards.begin(), cards.end());
	return vector<string>(noDup.begin(), noDup.end());
	
}

vector<string> getCards(vector<string>& cards, int num) {
	
	vector<int> indexesToGet;
	
	for (int i = 0; i < num;) {
		int randIndex = randUtil.randint(0, cards.size() - 1);
		if (!vecUtil::contains(indexesToGet, randIndex)) {
			indexesToGet.push_back(randIndex);
			i++;
		}
	}

	vector<string> cardsToGet;
	for (int i : indexesToGet) {
		cardsToGet.push_back(cards.at(i));
	}

	vecUtil::removeByIndexes(cards, indexesToGet);
	return cardsToGet;
	
}

void getCards(vector<string>& cards, string com) {

	int numToGet = ((parse::numArguments(com) == 0) ? 2 : parse::parseNumericalArgument(com));

	if (numToGet == 0) {
		cout << ANSI_RED << "\nNo cards were requested to get.\n" << ANSI_NORMAL;
		return;
	}

	if (numToGet > cards.size()) {
		cout << ANSI_RED << "\nSorry, not enough cards left.\n" << ANSI_NORMAL;
		return;
	}

	cout << "\n";
	for (string card : getCards(cards, numToGet)) {
		cout << card << "\n";
	}
	
}

void list(const vector<string>& cards, string com) {
	
	if (parse::numArguments(com) == 1 && parse::parseArgument(com) == "remaining") {

		if (cards.empty()) {
			cout << ANSI_RED << "\nThere are no remaining cards.\n" << ANSI_NORMAL;
			return;
		}
		
		cout << ANSI_BLUE << "\nListing remaining cards (" << cards.size() << "):\n\n" << ANSI_NORMAL;
		for (string c : cards) {
			cout << c << "\n";
		}
		
	} else if (parse::numArguments(com) == 1 && parse::parseArgument(com) == "all") {

		vector<string> ogCards = read();
		if (ogCards.empty()) {
			cout << ANSI_RED << "\nThere are no cards in the original card pool.\n" << ANSI_NORMAL;
			return;
		}
		
		cout << ANSI_BLUE << "\nListing all cards (" << ogCards.size() << "):\n\n" << ANSI_NORMAL;
		for (string c : ogCards) {
			cout << c << "\n";
		}
		
	} else if (parse::numArguments(com) == 1 && parse::parseArgument(com) == "categories") {

		cout << ANSI_BLUE << "\nListing all categories (" << CATEGORY_MAP.size() << "):\n\n" << ANSI_NORMAL;
		for (const auto& [cat, catCards] : CATEGORY_MAP) {
			cout << cat << ANSI_YELLOW << " (" << ANSI_MAGENTA << catCards.size() << ANSI_YELLOW << " cards)\n" << ANSI_NORMAL;
		}
		
	} else {
		
		cout << ANSI_RED << "\nGive a proper argument for " << ANSI_YELLOW << "/list" << ANSI_RED << ".\n" << ANSI_NORMAL;
		
	}
	
}

void help() {
	
	cout << ANSI_BLUE
		
			 << "\n*** COMMANDS ***\n\n"
		
			 << "/get <num> : get the next num cards to draft, if no argument then defaults to 2\n"
			 << "/draft : start a classic draft\n"
			 << "/mega : start a mega draft\n"
			 << "/list <remaining|all|categories> : display a list of remaining cards, all cards, or all available categories in the draft\n"
			 << "/left : display how many cards are left in the card pool\n"
			 << "/reset : reset the card pool to what it looked like when the draft started\n"
			 << "/help : display the program's help menu\n"
			 << "/exit : terminate the program\n\n"
		
			 << "*** CARD POOL EDITING ***\n\n"
		
			 << "[small spells] : all small damaging spells\n"
			 << "[large spells] : all large damaging spells (including Freeze)\n"
			 << "[non-damging spells] : all non-damaging spells\n"
			 << "[spells] : all spells\n\n"

			 << "[damaging buildings] : all damaging buildings (excluding Cannon Cart)\n"
			 << "[spawner buildings] : all spawner-like buildings (including Goblin Drill, Goblin Cage, and Elixir Collector)\n"
			 << "[buildings] : all buildings\n\n"

			 << "[swarms] : all swarms, i.e. troops that can be killed by a small spell (excluding Rascals)\n"
			 << "[lower glass cannons] : all lower glass cannons, i.e. troops that can be killed by Poison\n"
			 << "[upper glass cannons] : all upper glass cannons, i.e. troops that can be killed by Lightning\n"
			 << "[glass cannons] : all glass cannons\n\n"
		
			 << "[lower mini-tanks] : all lower mini-tanks, i.e. troops that can be killed by Rocket\n"
			 << "[upper mini-tanks] : all upper mini-tanks, i.e. troops that can be killed by Rocket + a small spell\n"
			 << "[mini-tanks] : all mini-tanks\n"
			 << "[tanks] : all tanks, i.e. troops that cannot be killed by Rocket + a small spell\n\n"
		
			 << "[troops] : all troops\n"
			 << "[all] : all cards\n\n"

			 << "There are some other categories that you can try:\n"
			 << "[air troops], [tower chasers], [champions], [goblins], [barbarians], [archers], [ice], [dragons], [magic],\n"
			 << "[target air(+)], [win conditions(+)], [group of troops(+)], [splash(+)], [skeletons(+)], [fire(+)], [electro(+)], [royal(+)].\n"
			 << "The plus sign in brackets indicates that there are two categories -\n"
			 << "a less complete one without the plus sign, and a more complete one with the plus sign.\n"
		   << "For example, [royal] contains cards such as Royal Ghost, Royal Recruits, Princess, and Prince,\n"
			 << "while [royal+] includes all the cards that [royal] contains plus some more such as Guards and Knight.\n\n"

			 << "THE FOLLOWING ONLY HAVE ANGLE BRACKETS FOR DEMONSTRATION! DO NOT PUT ANGLE BRACKETS IN PRACTICE!\n"
			 << "<card> : just a single card\n"
			 << "#<comment> : a comment that will be ignored\n"
			 << "-<card> : a card that will be excluded\n"
			 << "-[<category>] : an entire category that will be excluded\n"
			 << "*<card> : a card that will be force-added, i.e. will override any past or future removals\n"
			 << "*[<card>] : forcing adding, but for a category\n\n"

			 << "*** COMMANDS FOR CARD POOL EDITING ***\n\n"

			 << "/define$<new_cat>$list$<card1>$<card2>$[<cat1>]$[<cat2>]$... :\n"
			 << "defines or redefines a new category from listing existing cards and/or categories\n\n"
			 << "/define$<new_cat>$except$[<category>]$<except_card1|[except_category1]>$... :\n"
			 << "defines or redefines a new category from the first given card/category, except for any found in the second given one\n\n"
			 << "/define$<new_cat>$isec$[<cat1>]$[<cat2>]$... :\n"
			 << "defines or redefines a new category from the intersection of the given existing categories\n\n"

			 << "/select$<category>$<number> : ramdomly select a given number of cards from the given category\n"
		
			 << ANSI_NORMAL;
	
}

void classicDraft(vector<string>& cards) {
	
	if (cards.size() < 16) {
		cout << ANSI_RED << "\nNot enough cards remaining for a classic draft. You need at least 16 cards.\n" << ANSI_NORMAL;
		return;
	}

	vector<string> ogCards = cards;
	vector<string> player1Cards, player2Cards;
	
	map<int, int> stageToPlayer = mapUtil::makeMapFromVectorOfPairs(vector<pair<int, int>>({
		{1, 1}, {2, 2}, {3, 2}, {4, 1}, {5, 1}, {6, 2}, {7, 2}, {8, 1}
	}));
	
	for (int i = 1; i <= 8; i++) {

		cout << "\n*** CLASSIC DRAFT, CARD PICKING, STAGE " << i << "/8 ***\n";

		int playerNumber = stageToPlayer.at(i);
		auto playerColour = ((playerNumber == 1) ? ANSI_BLUE : ANSI_MAGENTA);
		cout << playerColour << "\nPlayer " << playerNumber << ", please select one of the following 2 cards:\n\n" << ANSI_NORMAL;
		
		auto cardChoices = getCards(cards, 2);
		cout << cardChoices.front() << "\n" << cardChoices.back() << "\n\n"
				 << ANSI_YELLOW << "What do you choose? Enter your choice here: " << ANSI_GREEN;

		string input;
		getline(cin >> ws, input);

		while (!vecUtil::contains(cardChoices, input)) {
			cout << ANSI_RED << "The given input is not one of the choices. Enter a valid choice: " << ANSI_GREEN;
			getline(cin >> ws, input);
		}

		auto opponentColour = ((playerColour == ANSI_BLUE) ? ANSI_MAGENTA : ANSI_BLUE);
		string otherCard = (cardChoices.front() == input) ? cardChoices.back() : cardChoices.front();
		
		cout << playerColour << "\nPlayer " << playerNumber << " received " << input << ".\n"
				 << opponentColour << "Player " << (3 - playerNumber) << " received " << otherCard << ".\n" << ANSI_NORMAL;

		if (playerNumber == 1) {
			player1Cards.push_back(input);
			player2Cards.push_back(otherCard);
		} else {
			player2Cards.push_back(input);
			player1Cards.push_back(otherCard);
		}
		
	}

	cout << "\n*** CLASSIC DRAFT, CARD PICKING, DECK SUMMARY ***\n"
			 << ANSI_BLUE << "\nPlayer 1, your cards are:\n" << ANSI_NORMAL;
	for (string card : player1Cards) {
		cout << card << "\n";
	}

	cout << ANSI_MAGENTA << "\nPlayer 2, your cards are:\n" << ANSI_NORMAL;
	for (string card : player2Cards) {
		cout << card << "\n";
	}

	cards = ogCards; // classic draft will automatically reset the card pool after the draft stage is complete
	
}

void printDraftBoard(const vector<string>& draftBoard, const vector<string>& p1, const vector<string>& p2) {

	vector<string> formattedBoard = strUtil::fillSpacesToMaxPlusN(draftBoard, 3);

	for (int i = 0; i < formattedBoard.size(); i++) {

		if (i % 6 == 0) {
			cout << "\n";
		}

		if (vecUtil::contains(p1, draftBoard.at(i))) {
			cout << ANSI_BLUE;
		} else if (vecUtil::contains(p2, draftBoard.at(i))) {
			cout << ANSI_MAGENTA;
		} else {
			cout << ANSI_YELLOW;
		}

		cout << formattedBoard.at(i) << ANSI_NORMAL;

		if (i % 6 == 5) {
			cout << "\n";
		}
		
	}
	
}

bool canPickThisCard(const vector<string>& draftBoard, const vector<string>& p1, const vector<string>& p2, string card) {
	return vecUtil::contains(draftBoard, card) && !vecUtil::contains(p1, card) && !vecUtil::contains(p2, card);
}

void megaDraft(vector<string>& cards) {
	
	if (cards.size() < 36) {
		cout << ANSI_RED << "\nNot enough cards remaining for a mega draft. You need at least 36 cards.\n" << ANSI_NORMAL;
		return;
	}

	vector<string> ogCards = cards;
	vector<string> draftBoard = getCards(cards, 36);
	vector<string> player1Cards, player2Cards;

	map<int, int> stageToPlayer = mapUtil::makeMapFromVectorOfPairs(vector<pair<int, int>>({
		{1, 1}, {2, 2}, {3, 2}, {4, 1}, {5, 1}, {6, 2}, {7, 2}, {8, 1},
		{9, 1}, {10, 2}, {11, 2}, {12, 1}, {13, 1}, {14, 2}, {15, 2}, {16, 1}
	}));

	for (int i = 1; i <= 16; i++) {

		cout << "\n*** MEGA DRAFT, CARD PICKING, STAGE " << i << "/16 ***\n";
		printDraftBoard(draftBoard, player1Cards, player2Cards);

		int playerNumber = stageToPlayer.at(i);
		auto playerColour = ((playerNumber == 1) ? ANSI_BLUE : ANSI_MAGENTA);
		cout << playerColour << "\nPlayer " << playerNumber << ", please select a card from the draft board and enter your choice here: " << ANSI_GREEN;

		string input;
		getline(cin >> ws, input);

		while (!canPickThisCard(draftBoard, player1Cards, player2Cards, input)) {
			cout << ANSI_RED << "The given input cannot be selected as a card for your deck. Enter a different card: " << ANSI_GREEN;
			getline(cin >> ws, input);
		}

		cout << playerColour << "\nPlayer " << playerNumber << " received " << input << ".\n" << ANSI_NORMAL;

		if (playerNumber == 1) {
			player1Cards.push_back(input);
		} else {
			player2Cards.push_back(input);
		}
		
	}

	cout << "\n*** MEGA DRAFT, CARD PICKING, DECK SUMMARY ***\n";
	printDraftBoard(draftBoard, player1Cards, player2Cards);
	
	cout << ANSI_BLUE << "\nPlayer 1, your cards are:\n" << ANSI_NORMAL;
	for (string card : player1Cards) {
		cout << card << "\n";
	}

	cout << ANSI_MAGENTA << "\nPlayer 2, your cards are:\n" << ANSI_NORMAL;
	for (string card : player2Cards) {
		cout << card << "\n";
	}

	cards = ogCards; // mega draft will automatically reset the card pool after the draft stage is complete
	
}

void defineNewCategory(string com) {

	vector<string> arguments = parse::parseAllArguments(com, false, '$');
	string newCatName = arguments.at(0);
	string defineOp = arguments.at(1);
	set<string> newCatCards;

	if (defineOp == "list") {

		for (int i = 2; i < arguments.size(); i++) {
			string arg = arguments.at(i);
			if (strUtil::beginsWith(arg, "[")) {
				for (string card : CATEGORY_MAP.at(arg.substr(1, arg.size() - 2))) {
					newCatCards.insert(card);
				}
			} else {
				newCatCards.insert(arg);
			}
		}
		
	} else if (defineOp == "except") {

		string og = arguments.at(2).substr(1, arguments.at(2).size() - 2);
		newCatCards.insert(CATEGORY_MAP.at(og).begin(), CATEGORY_MAP.at(og).end());

		for (int i = 3; i < arguments.size(); i++) {
			string except = arguments.at(i);
			if (strUtil::beginsWith(except, "[")) {
				for (string card : CATEGORY_MAP.at(except.substr(1, except.size() - 2))) {
					newCatCards.erase(card);
				}
			} else {
				newCatCards.erase(except);
			}
		}
		
	} else if (defineOp == "isec") {

		string og = arguments.at(2).substr(1, arguments.at(2).size() - 2);
		newCatCards.insert(CATEGORY_MAP.at(og).begin(), CATEGORY_MAP.at(og).end());

		for (int i = 3; i < arguments.size(); i++) {
			string isec = arguments.at(i).substr(1, arguments.at(i).size() - 2);
			set<string> isecResult;
			for (string card : newCatCards) {
				if (vecUtil::contains(CATEGORY_MAP.at(isec), card)) {
					isecResult.insert(card);
				}
			}
			newCatCards = isecResult;
		}
		
	}

	if (CATEGORY_MAP.count(newCatName)) {
		CATEGORY_MAP.at(newCatName) = vector<string>(newCatCards.begin(), newCatCards.end());
	} else {
		CATEGORY_MAP.insert({newCatName, vector<string>(newCatCards.begin(), newCatCards.end())});
	}
	
}

void select(string com, vector<string>& preparsed) {
	string category = parse::parseArgument(com, 1, '$');
	int num = parse::parseNumericalArgument(com, 2, '$');
	vector<string> selected = getCards(CATEGORY_MAP.at(category), num);
	preparsed.insert(preparsed.end(), selected.begin(), selected.end());
}

void preparse() {

	vector<string> raw, preparsed;
	file::inputStrVecFrom(raw, "input.txt");

	for (string line : raw) {
		if (strUtil::beginsWith(line, "/define")) {
			defineNewCategory(line);
		} else if (strUtil::beginsWith(line, "/select")) {
			select(line, preparsed);
		} else {
			preparsed.push_back(line);
		}
	}

	file::outputVecTo(preparsed, "internal_use.txt");
	
}

void reset(vector<string>& cards) {
	initializeCategoryMap();
	preparse();
	cards = read();
	cout << "\nReset successful.\n";
}

int main() {

	initializeCategoryMap();
	preparse();
	vector<string> cards = read();
	string in;
	help();

	while (true) {

		cout << ANSI_YELLOW << "\n>>> " << ANSI_GREEN;
		getline(cin >> ws, in);
		cout << ANSI_NORMAL;

		if (parse::commandIs(in, "/get")) {
			getCards(cards, in);
		} else if (parse::commandIs(in, "/left")) {
			cout << "\nCards left: " << cards.size() << " out of " << read().size() << ".\n";
		} else if (parse::commandIs(in, "/reset")) {
			reset(cards);
		} else if (parse::commandIs(in, "/list")) {
			list(cards, in);
		} else if (parse::commandIs(in, "/draft")) {
			classicDraft(cards);
		} else if (parse::commandIs(in, "/mega")) {
			megaDraft(cards);
		} else if (parse::commandIs(in, "/help")) {
			help();
		} else if (parse::commandIs(in, "/exit")) {
			cout << ANSI_BLUE << "\nThanks and bye!\n\n" << ANSI_NORMAL;
			break;
		} else {
			cout << ANSI_RED << "\nInvalid command.\n" << ANSI_NORMAL;
		}
		
	}
	
}