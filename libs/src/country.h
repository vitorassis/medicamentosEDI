

struct country
{
    char name[50];
    country *prev, *next;
    personDescriptor people;
};

struct countryDescriptor
{
    country *start, *end, *current, *startSection, *endSection;
    int quantity, sectionQuantity;
};