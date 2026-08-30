#include <windows.h>
#include <iostream>
#include "digitclock.h"

int main()
{
    std::cout << "Digital Clock - Timezone Demo\n" << std::endl;
    
    // Get time in specific zones
    std::cout << "Time in EST: " << GetTimeInZone("EST") << std::endl;
    std::cout << "Time in JST: " << GetTimeInZone("JST") << std::endl;
    std::cout << "Time in UTC: " << GetTimeInZone("UTC") << std::endl;
    std::cout << std::endl;
    
    // Display all timezones
    DisplayClock();
    
    std::cout << "\nPress any key to exit...";
    std::cin.get();
    
    return 0;
}
