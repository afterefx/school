#!/bin/bash
######################################################
######################################################
#                     Functions                      #
######################################################
######################################################
displayAvailableSeats(){
    echo
    echo "    A  B  C  D  E  F" #seat letter reference
    for (( ROW = 1; ROW < 61; ROW++ ));
    do
        #check if row is 1 or 2 digits and display accordingly
        if [[ ROW -gt 9 ]]; then
            echo -n "$ROW  "
            rowDis=$ROW
        else
            rowDis="0$ROW"
            echo -n "$rowDis  "
        fi
        #for each seat on the row check if seat is open
        for SEATLET in A B C D E F
        do
            SEAT="$rowDis$SEATLET"
            AVAILABLE=`grep -c ^$SEAT$ seats`

            if [[ AVAILABLE -eq 1 ]]; then
                echo -n "O  ";
            else
                echo -n "X  ";
            fi
        done
        echo;
        echo;
    done
    echo "    A  B  C  D  E  F" #seat letter reference
    echo; echo "X is occupied";echo "O is available" #legend
    read #pause
}

header() {
    clear
    echo "=================================== "
    echo "||  $headerText"
    echo "=================================== "
    echo
}

mainMenu() {
    echo "1.) Reserve a seat"
    echo "2.) Search"
    echo "3.) Edit a Reservation"
    echo "4.) Cancel Reservation"
    echo "5.) Quit"

    echo -n "Pick an option: "; read MENUOPTION
}

reserve() {
    MENUOPTION=0
    CONFIRM='n'
    while [ $MENUOPTION != "q" ] #wait for user to quit menu
    do
        headerText="Make a reservation";
        header
        echo "Type the seat you would like to reserve"
        echo "in the format of FirstName LastName Seat"
        echo;
        echo "If you would like to see available seats"
        echo "type 'list' and hit enter to see a display";
        echo
        echo "Type q and hit enter to go back to";
        echo "the main menu";
        echo;
        echo -n "# "; read Option LastName seatSub

        if [[ $Option == "list" ]]; then #display seat list
            displayAvailableSeats
        elif [[ $Option != "q" ]]; then #reserve a seat
            FirstName=$Option
            moreSeats='y'
            nextSeat='72A'

            #input validation and check that seat is available
            if [[ `echo "$FirstName $LastName $seatSub" | grep -c '^\<[a-zA-Z]\+ \<[a-zA-Z]\+ \<\([0-5][0-9]\|60\)[A-F]'` == 1 && `grep -c "^$seatSub$" seats` == 1 ]]; then
                #allow multiple seats to be input till user is done
                while [[ $moreSeats == "y" && $nextSeat != "done" ]]
                do
                    #if $nextSeat is set to an actual seat set $seatsub to its
                    #value to be made into reservation
                    if [[ `echo "$nextSeat" | grep -c "^\([0-5][0-9]\|60\)[A-F]$"` == 1 ]]; then
                        seatSub=$nextSeat
                    fi
                    seatRes="$seatSub $FirstName $LastName"; #make a string of seat and name
                    CONFIRM='n'
                    echo -n "Are you sure that you would like to reserve seat $seatSub for $FirstName $LastName (y/n): ";
                    read CONFIRM
                    if [[ $CONFIRM == 'y' ]]; then #check for approval
                        cat seats | sed "s/$seatSub/$seatRes/" > tempSeats #make temp edit using sed
                        `cat tempSeats > seats` #pipe temporary file to clobber seats file
                        `rm tempSeats` #remove temporary file
                        echo "Seat $seatSub was reserved for $FirstName $LastName.";echo; #show result

                        #check to see if user needs to be prompted to enter more seats
                        if [[ `echo "$nextSeat" | grep -c "^\([0-5][0-9]\|60\)[A-F]$"` != 1 ]]; then
                            echo -n "Would you like to reserve more seats for $FirstName $LastName (y/n): "; read moreSeats
                        fi
                        if [[ $moreSeats == 'y' ]]; then #if user wants to reserve more seats give them the ability to
                            echo "Please enter your next seat to reserve. When finished type 'done' and hit enter"
                            echo -n "Seat: "; read nextSeat
                        fi
                    else #Cancel reservation
                        nextSeat="done"
                        echo "Reservation was cancelled"; read
                    fi
                done
            else #Error handling
                echo  "There was an error with your input. Either ";
                echo "the seat is already reserved or your input could";
                echo "not be parsed."
                echo; echo "Format: FirstName LastName Seat"
                read
            fi
        else #go back to main menu
            MENUOPTION="q"
        fi
    done
}

displayEmptyRows() {
    headerText="Empty Rows";
    header
    count=0 #used to keep track of items displayed
        for (( num = 0; num < 7; num++ )); do #for the first digit 0-6
            for (( num2 = 0; num2 < 10; num2++ )); do #and second digit 0-9
                if [[ `grep -c "^$num$num2[A-F]$" seats` == 6 ]]; then #if row is empty display it
                    echo -n  "$num$num2 "
                        count=$[$count+1]
                    if [[ $count == 10 ]]; then #every ten items display new row
                        count=0
                        echo
                    fi
                fi
            done
        done
        read
}

displayOccupiedRows() {
    headerText="Occupied Rows";
    header
    count=0 #used to keep track of items displayed
        for (( num = 0; num < 7; num++ )); do #for the first digit 0-6
            for (( num2 = 0; num2 < 10; num2++ )); do #and second digit 0-9
                occupiedNum="$num$num2"
                if [[ `grep -c "^$occupiedNum[A-F]$" seats` == 0 && occupiedNum -lt 61 ]]; then #if row is occupied display it
                    if [[ $occupiedNum != "00" ]]; then #filter out 00 from results
                        echo -n  "$occupiedNum "
                        count=$[$count+1]
                    fi
                    if [[ $count == 10 ]]; then #every ten items display new row
                        count=0
                        echo
                    fi
                fi
            done
        done
        read
}

searchLastName() {
    headerText="Search by last name";
    header

    while [[ $MENUOPTION != "q" ]]
    do
        echo "Enter a name to search for";
        echo
        echo "Type '1' to go back to the search menu";
        echo
        echo -n "# "; read searchName
        #if name searched for has the letters a-z or A-Z
        if [[ `echo $searchName | grep -c '^[a-zA-Z]\+$'` == 1 ]]; then
            #make sure results only show matches with the lastname (exclude firstname matches)
            grep "$searchName" seats | grep "^.\{3\} \<[a-zA-Z]\+ \<$searchName"
            read
        elif [[ $searchName == "1" ]]; then #Exit search
            MENUOPTION="q"
        else #Error handling
            echo "Only characters a-z are allowed"
            read
        fi
    done
    MENUOPTION="0"
}

searchSeat() {
    headerText="Search by seat";
    header

    while [[ $MENUOPTION != "q" ]]
    do
        echo "Enter a seat to search for";
        echo
        echo "Type '1' to go back to the search menu";
        echo
        echo -n "# "; read searchSeat
        #if name searched for has the letters a-z or A-Z
        if [[ `echo "$searchSeat" | grep -c '^\([0-5][0-9]\|60\)[A-F]$'` == 1 ]]; then
            if [[ `grep -c "$searchSeat \<[a-zA-Z]\+ \<[a-zA-Z]\+$" seats` == 1 ]]; then
                #display who has the seat
                seatName=`grep "$searchSeat" seats | sed "s/$searchSeat //"`
                echo "$searchSeat is occupied by $seatName"
            else
                echo "$searchSeat is an empty seat.";
            fi
            read
        elif [[ $searchSeat == "1" ]]; then #Exit search
            MENUOPTION="q"
        else #Error handling
            echo "Imporperly formatted seat. There are rows 1-60 and seats A-F on each row."
            read
        fi
    done
    MENUOPTION="0"

}

search() {
    MENUOPTION=0
    CONFIRM='n'
    while [ $MENUOPTION != "q" ]
    do
        headerText="Search";
        header
        echo "1.) Display plane seats"
        echo "2.) Display empty rows"
        echo "3.) Display occupied rows"
        echo "4.) Search entries by last name"
        echo "5.) Search entries by seat"
        echo "6.) Go back to main menu"
        echo
        echo "Enter your menu choice by entering";
        echo "a number and press enter";
        echo;
        echo -n "# "; read Option

        case $Option in
            1)
                displayAvailableSeats
                ;;
            2)
                displayEmptyRows
                ;;
            3)
                displayOccupiedRows
                ;;
            4)
                searchLastName
                ;;
            5)
                searchSeat
                ;;
            6)
                MENUOPTION="q"
                ;;
            *)
                ;;
        esac
    done
    MENUOPTION="0"
}

edit() {
    headerText="Edit a reservation";
    header
    read
}

delete() {
    headerText="Cancel a reservation";
    header
    read
}

progSTART() {
    MENUOPTION=0 #set menu option to 0
    while [ $MENUOPTION != 5 ] #wait for user to ask to end the program
        do
            headerText="Airline Seat Reservation";
            header
            mainMenu #display the main menu to show the user their options

            case "$MENUOPTION" in
                1) #Reserve a seat
                    reserve;
                ;;
                2) #Search
                    search;
                ;;
                3) #Edit a reservation
                    edit;
                ;;
                4) #Cancel Reservation
                    delete;
                ;;
                5) #Quit Program
                    ;;
                *) #Error Handling
                    MENUOPTION=0;
                ;;
            esac
        done
}

######################################################
######################################################
#               Procedural Program                   #
######################################################
######################################################
progSTART #call the main function
