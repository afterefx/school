#!/bin/bash
#-Seats are #1-60 and then A-F
#-May request seat
    #-Add entries (consists of: FirstName LastName Seat)
#-May move a seat
    #-May edit entries: name & seat
#-May cancel reservation
#-Seat can have multiple entries (1 person reserves 3 seats)
#-Display by row empty or taken
#-Search entries by lastName, by seat
#****MUST ALWAYS CONFIRM*****
#****Need a cancel button******

#Menu driven

###########################
#        Functions        #
###########################
displayAvailableSeats(){
    echo
    echo "   A B C D E F"
    for (( ROW = 1; ROW < 61; ROW++ ));
    do
        #check if row is 1 or 2 digits and display accordingly
        if [[ ROW -gt 9 ]]; then
            echo -n "$ROW "
            rowDis=$ROW
        else
            rowDis="0$ROW"
            echo -n "$rowDis "
        fi
        #for each seat on the row check if seat is open
        for SEATLET in A B C D E F
        do
            SEAT="$rowDis$SEATLET"
            AVAILABLE=`grep -c ^$SEAT$ seats`

            if [[ AVAILABLE -eq 1 ]]; then
                echo -n "O ";
            else
                echo -n "X ";
            fi
        done

        echo;
    done
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
    echo "2.) Edit a Reservation"
    echo "3.) Cancel Reservation"
    echo "4.) Quit"

    echo -n "Pick an option: "; read MENUOPTION
}
reserve() {
    MENUOPTION=0
    while [ $MENUOPTION != "q" ]
    do
        headerText="Make a reservation";
        header
        echo "Type your seat you would like to reserve"
        echo "in the format of FirstName LastName Seat"
        echo;
        echo "If you would like to see available seats"
        echo "type 'list' and hit enter to see a display";
        echo
        echo "Type q and hit enter to go back to";
        echo "the main menu";
        echo;
        echo -n "# "; read MENUOPTION

        if [[ $MENUOPTION == "list" ]]; then
            displayAvailableSeats
            read NULL
        elif [[ $MENUOPTION != "q" ]]; then
            #make entry
            echo -n "made entry"; read NULL
        fi
    done
}

edit() {
    headerText="Edit a reservation";
    header
}
delete() {
    headerText="Cancel a reservation";
    header
}
progSTART() {
    MENUOPTION=0
    while [ $MENUOPTION != 4 ]
        do
            headerText="Airline Seat Reservation";
            header
            mainMenu

            case "$MENUOPTION" in
                1) #Reserve a seat
                    clear
                    reserve;
                ;;
                2) #Edit a reservation
                    clear
                    edit;
                    read
                ;;
                3) #Cancel Reservation
                    clear
                    delete;
                    read
                ;;
                4)
                    ;;
                *)
                    MENUOPTION=0;
                ;;
            esac
        done
}
###########################
#    Procedural Program   #
###########################
progSTART
