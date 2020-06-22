#include "country.h"

countryDescriptor createCountryDescriptor(){
    countryDescriptor countries;
    countries.startSection = countries.endSection = countries.start = countries.end = countries.current = NULL;
    countries.quantity = countries.sectionQuantity = 0;

    return countries;
}

country *createCountry(char name[]){
    country *_country = new country;
    strcpy(_country->name, name);
    _country->next = _country->prev = NULL;
    _country->people = createPersonDescriptor();

    return _country;
}

void insertCountry(countryDescriptor &countries, char name[]){
	
	// inicializando o ponteiro atual com o inicio da lista
	moveToFirstCountry(countries);
	
	// garantindo a nao insercao de elementos nulos (sem texto)
	if(strlen(name) > 0) {
		
		// novo pais que sera inserido
		country *_country = createCountry(name);
		
		if(isCountriesEmpty(countries)) {
			// lista nula
			countries.start = _country;
			countries.end = _country; 

			countries.quantity = 1;
			countries.current = _country;
		} else {
			// lista com nodos
			// str1 > str2 > 0
			// str1 == str2 == 0
			// str1 < str2 < 0
			
			while(getCurrentCountry(countries) != NULL && stricmp(name, getCurrentCountry(countries)->name) > 0){
				moveToNextCountry(countries);
			}
			
			if((countries.quantity > 1 && getCurrentCountry(countries) == NULL) || 
				(countries.quantity == 1 && stricmp(name, getCurrentCountry(countries)->name) > 0)) {
				// insercao no final da lista
				_country->prev = countries.end;  
				countries.end->next = _country;  

				countries.end = _country;
				countries.quantity++;
				countries.sectionQuantity++;
				countries.current = _country;
			} else {
				// insercao no inicio ou no meio da lista
				// garantindo a nao insercao de um elemento repetido
				if(stricmp(name, getCurrentCountry(countries)->name) != 0){
					if(isCountryTheFirst(countries)) {
						// insercao no inicio da lista
						_country->next = countries.current;
						countries.current->prev = _country;
						
						countries.start = _country;
					} else {
						// insercao no meio da lista
						_country->next = countries.current;
						_country->prev = countries.current->prev;
					
						countries.current->prev->next = _country;
						countries.current->prev = _country;
					}
					
					countries.quantity++;
					countries.sectionQuantity++;
					countries.current = _country;
				}
			}
		}
	}
}

country *getFirstCountry(countryDescriptor countries){
    return countries.start;
}
country *getLastCountry(countryDescriptor countries){
    return countries.end;
}

country *getFirstCountrySection(countryDescriptor countries){
    return countries.startSection;
}
country *getLastCountrySection(countryDescriptor countries){
    return countries.endSection;
}

country *getCurrentCountry(countryDescriptor countries){
    return countries.current;
}
country *getNextCountry(countryDescriptor countries){
    return getCurrentCountry(countries)->next;
}
country *getPreviousCountry(countryDescriptor countries){
    return getCurrentCountry(countries)->prev;
}

void moveToNextCountry(countryDescriptor &countries){
//    if(!isCountryTheLast(countries)) {
        countries.current = countries.current->next;
//    }
}
void moveToPreviousCountry(countryDescriptor &countries){
//    if(!isCountryTheFirst(countries)) {
        countries.current = countries.current->prev;
//    }
}

void moveToFirstCountry(countryDescriptor &countries){
    countries.current = countries.start;
}

void moveToFirstCountrySection(countryDescriptor &countries){
	countries.current = countries.startSection;
}

void moveToLastCountry(countryDescriptor &countries){
    countries.current = countries.end;
}
void moveToLastCountrySection(countryDescriptor &countries){
	countries.current = countries.endSection;
}

int isCountryTheLast(countryDescriptor countries){
    return countries.current == countries.end;
}

int isCountryTheFirst(countryDescriptor countries){
    return countries.current == countries.start;
}

int isEndOfCountries(countryDescriptor countries){
    return countries.current == NULL;
}

int isEndOfCountriesSection(countryDescriptor countries){
	return isEndOfCountries(countries) || countries.current == countries.endSection->next;
}

int isCountriesEmpty(countryDescriptor countries){
	return countries.quantity == 0;
}

country *getCountryByNode(countryDescriptor countries, int nodes){
	country *_country = getFirstCountry(countries);
    while(nodes-- > 0) _country = _country->next;

	return _country;
}

void getCountriesFirstLetters(countryDescriptor countries, char letters[], int &size){
	size = 0;
	char actual= 0;
	moveToFirstCountry(countries);

	while(!isEndOfCountries(countries)){
		if(actual !=getCurrentCountry(countries)->name[0]){
			actual = getCurrentCountry(countries)->name[0];
			letters[size++] = actual;
		}
		moveToNextCountry(countries);	
	}
	strcat(letters, "\0");
}

void getCountriesFromFirstLetter(countryDescriptor &countries, char letter){
	moveToFirstCountry(countries);

	while(letter != getCurrentCountry(countries)->name[0])
		moveToNextCountry(countries);
	countries.startSection = getCurrentCountry(countries);

	while(!isEndOfCountries(countries) && letter == getCurrentCountry(countries)->name[0]) 
		moveToNextCountry(countries);
	countries.endSection = !isEndOfCountries(countries) ? getCurrentCountry(countries)->prev : getLastCountry(countries);
	
	moveToFirstCountrySection(countries);

	countries.sectionQuantity = 0;
	while(!isEndOfCountriesSection(countries)){ 
		countries.sectionQuantity++;
		moveToNextCountry(countries);
	}
	moveToFirstCountrySection(countries);
}

country *getCountryByNodeInSection(countryDescriptor countries, int nodes){
	country *_country = getFirstCountrySection(countries);
    while(nodes-- > 0) _country = _country->next;

	return _country;	
}

void removeCountry(countryDescriptor &countries, country *toRemove){
	while(getCountryPeople(toRemove).quantity > 0)
		removePerson(toRemove->people, getFirstPerson(getCountryPeople(toRemove)));
	
	if(countries.quantity == 1)
		countries = createCountryDescriptor();
	else{
		if(countries.start == toRemove){
			countries.start = toRemove->next;
			toRemove->next->prev = NULL;
		}
		else if(countries.end == toRemove){
			countries.end = toRemove->prev;
			toRemove->prev->next = NULL;
		}
		else{
			toRemove->next->prev = toRemove->prev;
			toRemove->prev->next = toRemove->next;
		}

		countries.quantity--;
	}
	countries.current = toRemove->next;
	delete(toRemove);
}

void wipeTrashCountryData(countryDescriptor &countries){
	personDescriptor people;
	if(countries.quantity > 0){
		moveToFirstCountry(countries);
		while(!isEndOfCountries(countries)){
			people = getCurrentCountry(countries)->people;
			wipeTrashPeopleData(people);
			getCurrentCountry(countries)->people = people;

			if(getCountryPeople(getCurrentCountry(countries)).quantity == 0){
				removeCountry(countries, getCurrentCountry(countries));
			}
			
			if(!isEndOfCountries(countries))
				moveToNextCountry(countries);
		}
	}
}

void searchCountries(countryDescriptor &countries, char search[]){
	moveToFirstCountry(countries);
	showToast("PROCURANDO...", TOAST_WARNING);
	while(!isEndOfCountries(countries) && !checkSearch(getCurrentCountry(countries)->name, search)){
		moveToNextCountry(countries);
		
	}
	if(!isEndOfCountries(countries) && checkSearch(getCurrentCountry(countries)->name, search)){
		showToast("PAIS ENCONTRADO!", TOAST_SUCCESS);
		
		countries.startSection = getCurrentCountry(countries);
		
		while(!isEndOfCountries(countries) && checkSearch(getCurrentCountry(countries)->name, search)) {
			moveToNextCountry(countries);
		}
		countries.endSection = !isEndOfCountries(countries) ? getCurrentCountry(countries)->prev : getLastCountry(countries);

		
		moveToFirstCountrySection(countries);

		countries.sectionQuantity = 0;

		while(!isEndOfCountriesSection(countries)){ 
			countries.sectionQuantity++;
			moveToNextCountry(countries);
		}
		moveToFirstCountrySection(countries);
	}
	else{
		countries.endSection = countries.startSection = NULL;
		countries.sectionQuantity = 0;
	}
	
}