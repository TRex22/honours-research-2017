/*Card class*/

class Card
{
	private:


	public:
		std::string cardName = "Empty Card";
		std::string imageLocation = "";
		int cardId = 0;
		int locationId = -1;
		// int chosenLocation = -1;
		int typeId = -1;
		int attackPoints = 0;
		int extraCards = 0;
		bool checkAgile = false;
		bool hasBeenPlayed = false;
		bool hasBeenSelected = false;

		Card();
		~Card();
		Card(Card* otherCard);
		Card(std::string cardName, int cardId, int locationId, int typeId, int attackPoints, int extraCards, std::string imageLocation);
		bool equals(Card otherCard);
		void copy(Card* otherCard);
		void copy(Card otherCard);
		void print();

		// bool operator==(Card rhs) const {return this->equals(rhs);}
};

Card::Card()
{
}

Card::~Card()
{
}

Card::Card(Card* otherCard)
{
	this->cardId = otherCard->cardId;
	this->locationId = otherCard->locationId;
	this->typeId = otherCard->typeId;
	this->attackPoints = otherCard->attackPoints;
	this->extraCards = otherCard->extraCards;
	this->imageLocation = otherCard->imageLocation;
}

Card::Card(std::string cardName, int cardId, int locationId, int typeId, int attackPoints, int extraCards, std::string imageLocation)
{
	this->cardName = cardName;
	this->cardId = cardId; //must start at 1
	this->locationId = locationId;
	this->typeId = typeId;
	this->attackPoints = attackPoints;
	this->extraCards = extraCards;
	this->imageLocation = imageLocation;
}

bool Card::equals(Card otherCard)
{
	if (this->cardId != otherCard.cardId)
		return false;

	if (this->locationId != otherCard.locationId)
		return false;

	if (this->typeId != otherCard.typeId)
		return false;

	if (this->attackPoints != otherCard.attackPoints)
		return false;

	if (this->extraCards != otherCard.extraCards)
		return false;

	return true;
}

void Card::copy(Card* otherCard)
{
	this->cardId = otherCard->cardId;
	this->locationId = otherCard->locationId;
	this->typeId = otherCard->typeId;
	this->attackPoints = otherCard->attackPoints;
	this->extraCards = otherCard->extraCards;
	this->imageLocation = otherCard->imageLocation;
}

void Card::copy(Card otherCard)
{
	this->cardId = otherCard.cardId;
	this->locationId = otherCard.locationId;
	this->typeId = otherCard.typeId;
	this->attackPoints = otherCard.attackPoints;
	this->extraCards = otherCard.extraCards;
	this->imageLocation = otherCard.imageLocation;
}

void Card::print()
{
	printf("| %s %d %d |", this->cardName.c_str(), this->attackPoints, this->extraCards);
}