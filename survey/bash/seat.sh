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
    echo "    A  B  C  D  E  F"
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
    echo "    A  B  C  D  E  F"
    echo; echo "X is occupied";echo "O is available"

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

        if [[ $Option == "list" ]]; then
            displayAvailableSeats
            read NULL
        elif [[ $Option != "q" ]]; then
            #let seatSub=`echo $MENUOPTION | awk '{print $3}'`
            FirstName=$Option
            moreSeats='y'
            nextSeat='72A'
            if [[ `echo "$FirstName $LastName $seatSub" | grep -c '^\<[a-zA-Z]\+ \<[a-zA-Z]\+ \<\([0-5][0-9]\|60\)[A-F]'` == 1 && `grep -c "^$seatSub$" seats` == 1 ]]; then
                while [[ $moreSeats == "y" && $nextSeat != "done" ]]
                do
                    if [[ `echo "$nextSeat" | grep -c "^\([0-5][0-9]\|60\)[A-F]$"` == 1 ]]; then
                        seatSub=$nextSeat
                    fi
                    seatRes="$seatSub $FirstName $LastName";
                    cat seats | sed "s/$seatSub/$seatRes/" > tempSeats
                    `cat tempSeats > seats`
                    `rm tempSeats`
                    echo "Seat $seatSub was reserved for $FirstName $LastName.";echo;

                    if [[ `echo "$nextSeat" | grep -c "^\([0-5][0-9]\|60\)[A-F]$"` != 1 ]]; then
                        echo -n "Would you like to reserve more seats for $FirstName $LastName (y/n): "; read moreSeats
                    fi
                    if [[ $moreSeats == 'y' ]]; then
                        echo "Please enter your next seat to reserve. When finished type 'done' and hit enter"
                        echo -n "Seat: "; read nextSeat
                    fi
                done
            else
                echo  "There was an error with your input. Either ";
                echo "the seat is already reserved or your input could";
                echo "not be parsed."
                echo; echo "Format: FirstName LastName Seat"
                read NULL
            fi
        else
            MENUOPTION="q"
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
