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

    echo "Pick an option: "; read MENUOPTION
}
reserve() {
    headerText="Make a reservation";
    header
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
                    read
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
