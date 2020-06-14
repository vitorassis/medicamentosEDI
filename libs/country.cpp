#include "country.h"

countryDescriptor createCountryDescriptor(){
    countryDescriptor countries;
    countries.start = countries.end = countries.current = NULL;
    countries.quantity = 0;

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

void moveToLastCountry(countryDescriptor &countries){
    countries.current = countries.end;
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

int isCountriesEmpty(countryDescriptor countries){
	return countries.quantity == 0;
}

country *getCountryByNode(countryDescriptor countries, int nodes){
	country *_country = getFirstCountry(countries);
    while(nodes-- > 0) _country = _country->next;

	return _country;
}