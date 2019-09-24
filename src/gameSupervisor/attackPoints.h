/*playerinfo class*/
/*This is info for a specific player
	Lists of cards

	CurrentDeck <- starts full
	DiscardPile <- starts empty

*/

class AttackPoints
{
	private:
		int baseAttackPoints = 0;
		int currentAttackPoints = 0;
		int apModifier = 0;

		int closeModifier = 0;
		int rangedModifier = 0;
		int siegeModifier = 0;

	public:
		bool closeWeatherOn = false;
		bool rangedWeatherOn = false;
		bool siegeWeatherOn = false;

		bool closeHorn = false;
		bool rangedHorn = false;
		bool siegeHorn = false;

		int closeAttackPoints = 0;
		int	rangedAttackPoints = 0;
		int	siegeAttackPoints = 0;

		int closeCardCount = 0;
		int rangedCardCount = 0;
		int siegeCardCount = 0;

		AttackPoints();
		~AttackPoints();
		bool equals(AttackPoints otherAttackPoints);
		int getBaseAttackPoints();
		void getWeather();
		void calculateCurrentAttackPoints();
		int getCurrentAttackPoints();
		void resetCurrentAttackPoints();
		void clearAttackPointsModifiers();
		void print();

		void copy(AttackPoints ap);
		int getApModifier();
		int getCloseModifier();
		int getRangedModifier();
		int getSiegeModifier();
};

AttackPoints::AttackPoints()
{
}

AttackPoints::~AttackPoints()
{
}

bool AttackPoints::equals(AttackPoints otherAttackPoints)
{
	if(this->baseAttackPoints != otherAttackPoints.baseAttackPoints)
		return false;
	if(this->currentAttackPoints != otherAttackPoints.currentAttackPoints)
		return false;
	if(this->apModifier != otherAttackPoints.apModifier)
		return false;

	if(this->closeModifier != otherAttackPoints.closeModifier)
		return false;
	if(this->rangedModifier != otherAttackPoints.rangedModifier)
		return false;
	if(this->siegeModifier != otherAttackPoints.siegeModifier)
		return false;

	if(this->closeWeatherOn != otherAttackPoints.closeWeatherOn)
		return false;
	if(this->rangedWeatherOn != otherAttackPoints.rangedWeatherOn)
		return false;
	if(this->siegeWeatherOn != otherAttackPoints.siegeWeatherOn)
		return false;

	if(this->closeHorn != otherAttackPoints.closeHorn)
		return false;
	if(this->rangedHorn != otherAttackPoints.rangedHorn)
		return false;
	if(this->siegeHorn != otherAttackPoints.siegeHorn)
		return false;

	if(this->closeCardCount != otherAttackPoints.closeCardCount)
		return false;
	if(this->rangedCardCount != otherAttackPoints.rangedCardCount)
		return false;
	if(this->siegeCardCount != otherAttackPoints.siegeCardCount)
		return false;

	return true;
}

int AttackPoints::getBaseAttackPoints()
{
	this->baseAttackPoints = this->closeAttackPoints + this->rangedAttackPoints + this->siegeAttackPoints;
	return baseAttackPoints;
}

void AttackPoints::calculateCurrentAttackPoints()
{

	this->currentAttackPoints = 0;
	if(this->closeWeatherOn && this->closeHorn)
	{
		this->currentAttackPoints += 2*this->closeCardCount;
	}
	else if(this->closeHorn)
	{
		this->currentAttackPoints += 2*this->closeAttackPoints;
	}
	else if(this->closeWeatherOn)
	{
		this->currentAttackPoints += this->closeCardCount;
	}
	else
	{
		this->currentAttackPoints += this->closeAttackPoints;
	}

	if(this->rangedWeatherOn && this->rangedHorn)
	{
		this->currentAttackPoints += 2*this->rangedCardCount;
	}
	else if(this->rangedHorn)
	{
		this->currentAttackPoints += 2*this->rangedAttackPoints;
	}
	else if(this->rangedWeatherOn)
	{
		this->currentAttackPoints += this->rangedCardCount;
	}
	else
	{
		this->currentAttackPoints += this->rangedAttackPoints;
	}

	if(this->siegeWeatherOn && this->siegeHorn)
	{
		this->currentAttackPoints += 2*this->siegeCardCount;
	}
	else if(this->siegeHorn)
	{
		this->currentAttackPoints += 2*this->siegeAttackPoints;
	}
	else if(this->siegeWeatherOn)
	{
		this->currentAttackPoints += this->siegeCardCount;
	}
	else
	{
		this->currentAttackPoints += this->siegeAttackPoints;
	}
}

int AttackPoints::getCurrentAttackPoints()
{
	if(!(this->closeWeatherOn || this->rangedWeatherOn || this->siegeWeatherOn
		|| this->closeHorn || this->rangedHorn || this->siegeHorn))
		return getBaseAttackPoints();

	calculateCurrentAttackPoints();
	return this->currentAttackPoints;
}

void AttackPoints::resetCurrentAttackPoints()
{
	this->apModifier = 0;
	this->closeModifier = 0;
	this->rangedModifier = 0;
	this->siegeModifier = 0;

	this->closeCardCount = 0;
	this->rangedCardCount = 0;
	this->siegeCardCount = 0;

	this->baseAttackPoints = 0;
	this->closeAttackPoints = 0;
	this->rangedAttackPoints = 0;
	this->siegeAttackPoints = 0;

	this->closeHorn = false;
	this->rangedHorn = false;
	this->siegeHorn = false;

	this->currentAttackPoints = this->baseAttackPoints;
}

void AttackPoints::clearAttackPointsModifiers()
{
	this->apModifier = 0;
	this->closeModifier = 0;
	this->rangedModifier = 0;
	this->siegeModifier = 0;

	this->closeCardCount = 0;
	this->rangedCardCount = 0;
	this->siegeCardCount = 0;

	this->closeWeatherOn = false;
	this->rangedWeatherOn = false;
	this->siegeWeatherOn = false;

	this->currentAttackPoints = this->baseAttackPoints;
}

void AttackPoints::print()
{
	printf("Attack Points:\n");
	printf("base\tcurrent\tmodifier\n");
	printf("%d\t%d\n", getBaseAttackPoints(), getCurrentAttackPoints());
	printf(SINGLE_LINE);
	printf("AP: close\tranged\tsiege\n");
	printf("    %d\t%d\t%d\n", this->closeAttackPoints, this->rangedAttackPoints, this->siegeAttackPoints);
	printf(SINGLE_LINE);
	printf("Mod: close\tranged\tsiege\n");
	printf("     %d\t%d\t%d\n", this->closeModifier, this->rangedModifier, this->siegeModifier);
	printf(DOUBLE_LINE);
}



void AttackPoints::copy(AttackPoints ap)
{
	this->baseAttackPoints = ap.getBaseAttackPoints();
	this->currentAttackPoints = ap.getCurrentAttackPoints();
	this->apModifier = ap.getApModifier();
	this->closeModifier = ap.getCloseModifier();
	this->rangedModifier = ap.getRangedModifier();
	this->siegeModifier = ap.getSiegeModifier();

	this->closeWeatherOn = ap.closeWeatherOn;
	this->rangedWeatherOn = ap.rangedWeatherOn;
	this->siegeWeatherOn = ap.siegeWeatherOn;
	this->closeHorn = ap.closeHorn;
	this->rangedHorn = ap.rangedHorn;
	this->siegeHorn = ap.siegeHorn;
	this->closeAttackPoints = ap.closeAttackPoints;
	this->rangedAttackPoints = ap.rangedAttackPoints;
	this->siegeAttackPoints = ap.siegeAttackPoints;
	this->closeCardCount = ap.closeCardCount;
	this->rangedCardCount = ap.rangedCardCount;
	this->siegeCardCount = ap.siegeCardCount;
}

int AttackPoints::getApModifier()
{
	return this->apModifier;
}

int AttackPoints::getCloseModifier()
{
	return this->closeModifier;
}

int AttackPoints::getRangedModifier()
{
	return this->rangedModifier;
}

int AttackPoints::getSiegeModifier()
{
	return this->siegeModifier;
}