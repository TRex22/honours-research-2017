rapidjson::Document openJsonFile(std::string filepath)
{
	// Taken from: http://rapidjson.org/md_doc_stream.html
	#if (DEBUG)
		printf("Open Json File.\n");
	#endif

	FILE* fp = fopen(filepath.c_str(), "r"); // non-Windows use "r"
	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	rapidjson::Document d;

	#if (DEBUG)
		printf("Parse Json File.\n");
	#endif
	// d.ParseStream(is);
	d.ParseStream<0, UTF8<>, FileReadStream>(is);
	fclose(fp);

	return d;
}

std::vector<Card> convertJsonToVectorDeck(std::string path)
{
	std::vector<Card> deck;
	rapidjson::Document doc = openJsonFile(path);
    
	#if (DEBUG)
		printf("Open Card Array Object.\n");
	#endif

	// auto cardArr = doc["cards"].GetObject();
	const Value& cardArr = doc["cards"].GetArray();
	assert(cardArr.IsArray());

	#if (DEBUG)
		printf("Loop through cards.\n");
	#endif

	for(rapidjson::SizeType i = 0; i < cardArr.Size(); i++)
	{
		string cardName = cardArr[i].GetObject()["name"].GetString();
		int cardId = i; 
		int locationId = cardArr[i].GetObject()["locationId"].GetInt();
		int typeId = cardArr[i].GetObject()["typeId"].GetInt();
		int attackPoints = cardArr[i].GetObject()["attackPoints"].GetInt();
		int extraCards = cardArr[i].GetObject()["extraCards"].GetInt();
		string imageLocation = cardArr[i].GetObject()["imageLocation"].GetString();

		#if (DEBUG)
			printf("Make card and push to deck vector.\n");
		#endif

		Card newCard(cardName, cardId, locationId, typeId, attackPoints, extraCards, imageLocation);
		// std::cout<<"Test: "<<cardName<<std::endl;
		if(check_if_ignore_card(typeId)) // hack because for some reason using wrong cards
			deck.push_back(newCard);
	}

	return deck;
}

bool check_if_ignore_card(int typeId)
{
	if (typeId == rules(scorch))
		return false;
	if(typeId == rules(decoy))
		return false;

	return true;
}

void halt_execution(std::string message="")
{
	cout << message << endl;
    exit(EXIT_FAILURE);
}

void write_results_to_file (std::string filename, std::string results)
{
	ofstream file;
	file.open(filename.c_str(), ios::app);
		file << results;
	file.close();
}

void write_results_to_file (std::string filename, std::string header, std::string results)
{
	ofstream file;
	file.open(filename.c_str(), ios::app);
		file << header << results << endl;
	file.close();
}

void print_cmd_heading(std::string app_name)
{
	printf("%s\nJason Chalom 711985\n2017\n\n", app_name.c_str());
}

void print_usage(char *argv[])
{   
    printf("At least two parameters must be selected.\n-1 will denote inf value\n\n");
    printf("usage: %s --run_stats \n", argv[0]);
}

bool contains_string(std::string input, std::string str)
{
    if (input.find(str) != string::npos) {
        return true;
    } 
    return false;
}