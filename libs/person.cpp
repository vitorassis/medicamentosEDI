#include "person.h"

personDescriptor createPersonDescriptor(){
    personDescriptor people;
    people.current = people.start = NULL;
    people.quantity = 0;

    return people;
}


personDescriptor getCountryPeople(country *_country){
    personDescriptor people = _country->people;
    moveToFirstPerson(people);

    return people;
}

person *createPerson(char code[], char gender){
    person *_person = new person;
    //if(strlen(code) == 11 && (toupper(gender) == 'M' || toupper(gender) == 'F')){
        strcpy(_person->code, code);
        _person->gender = gender;
        _person->medicines = createMedicineDescriptor();
    //}

    return _person;
}

void insertPerson(country *_country, char code[], char gender){
    personDescriptor people = getCountryPeople(_country);
    //if(strlen(code) == 11 && (toupper(gender) == 'M' || toupper(gender) == 'F')){
        person *_person = createPerson(code, gender);

        person *aux = getFirstPerson(people);
        people.start = _person;
        _person->next = aux;
        people.quantity++;
        people.current = _person;
    //}
    _country->people = people;
}

person *getFirstPerson(personDescriptor people){
    return people.start;
}

person *getCurrentPerson(personDescriptor people){
    return people.current;
}

person *getNextPerson(personDescriptor people){
    return getCurrentPerson(people)->next;
}

void moveToNextPerson(personDescriptor &people){
    people.current = getNextPerson(people);
}

void moveToFirstPerson(personDescriptor &people){
    people.current = people.start;
}

int isPersonTheLast(personDescriptor people){
    return getCurrentPerson(people)->next == NULL;
}

int isPersonTheFirst(personDescriptor people){
    return getCurrentPerson(people) == getFirstPerson(people);
}

int isEndOfPeople(personDescriptor people){
    return getCurrentPerson(people) == NULL;
}

person *getPersonByNode(personDescriptor people, int nodes){
    person *_person = getFirstPerson(people);
    while(nodes-- > 0) _person = _person->next;

    return _person;
}

void removePerson(personDescriptor &people, person *toRemove){
    while(getPersonMedicines(toRemove).quantity > 0)
        removeMedicine(toRemove->medicines, getFirstMedicine(getPersonMedicines(toRemove)));

    if(people.quantity == 1)
        people = createPersonDescriptor();
    else{
        if(people.start == toRemove)
            people.start = toRemove->next;
        
        else{
            moveToFirstPerson(people);
            while(getCurrentPerson(people)->next != toRemove) moveToNextPerson(people);
            getCurrentPerson(people)->next = toRemove->next;
        }

        people.quantity--;
    }

    delete(toRemove);
}