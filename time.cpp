#include <stdio.h>
#include <time.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <curses.h>
#include <list>
#include <string>

using namespace std;

int main(int argc, char *argv[]){

    time_t seconds;

    int minutes, hours, days, years, decades, add;

    use_env(true);

    initscr();

    refresh();

    clear();

    list<string> randomnums;
    string holder;

    while(true){

        seconds = time(NULL);

        minutes = seconds / 60;
        hours = seconds / 3600;
        days = seconds / 86400;
        years = seconds / 31556926;
        decades = seconds / 315569260;

        refresh();
        printw("Time since January 1st, 1970:\n");
        refresh();
        printw("Seconds -> %d\n",seconds);
        refresh();
        printw("Minutes -> %d\n",minutes);
        refresh();
        printw("Hours   -> %d\n",hours);
        refresh();
        printw("Days    -> %d\n",days);
        refresh();
        printw("Years   -> %d\n",years);
        refresh();
        printw("Decades -> %d\n",decades);
        refresh();

        printw("All together now!\n");
        refresh();
        printw("Decades -> %d\n",decades);
        refresh();
        add = decades * 315569260;
        years = (seconds - add)/31556926;
        printw("Years   -> %d\n",years);
        refresh();
        add += years * 31556926;
        days = (seconds - add)/86400;
        printw("Days    -> %d\n",days);
        refresh();
        add += days * 86400;
        hours = (seconds - add)/3600;
        printw("Hours   -> %d\n",hours);
        refresh();
        add += hours * 3600;
        minutes = (seconds - add)/60;
        printw("Minutes -> %d\n",minutes);
        refresh();
        add += minutes * 60;
        seconds = seconds - add;
        printw("Seconds -> %d\n",seconds);
        refresh();

        for(int q = 0; q < 10; q++) 
        {
            holder += " " + 10*rand()/RAND_MAX;
        }
        holder += '\n';

        randomnums.push_front(holder);

        holder = "";

        list<string>::iterator it;

        /*
        for(it = randomnums.begin(); it != randomnums.end(); it++)
        {
            printw(*it);
            refresh();
        }
        */

        sleep(1);

        clear();

        if(randomnums.size() > 9)
            randomnums.pop_back();
    }

    endwin();

    return 0;
}
