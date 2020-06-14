

struct country
{
    char name[50];
    country *prev, *next;
    personDescriptor people;
};

struct countryDescriptor
{
    country *start, *end, *current;
    int quantity;
};