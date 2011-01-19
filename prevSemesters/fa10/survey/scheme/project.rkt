#lang racket
;;Christopher Carlisle
;;Scheme plane reservation system
;;Survey of Programming languages
;; 10/11/10

; Data definition :
;; (define-struct plane(seatno_list))
;; plane is a structure: (make-plane "13b")) where
;; seatno is a list of seats.
(define-struct plane(rows))

(define-struct rowSeats (number seatA seatB seatC seatD seatE seatF))
(define-struct seat (occupancy passenger))

; Data definition :
;; (define-struct passenger(firstName lastName))
;; passenger is a structure: (make-passenger "izzy" "islander")) where
;; firstName is a string, lastName is a string
(define-struct passenger(firstName lastName))


(define (make-reservation plane first last) (display ""))

(define (reserveMenu option)
 (and
 (and (display "
========================
== Make a reservation ==
========================

Type the seat you would like to reserve ")
 (let ([seatNum (read-line)])
   (and
    (display "First name for reservation:")
    (let
     ([tempFirst (read-line)])
     (and
      (display "Last name for reservation:")
      (let ([tempLast (read-line)]) (make-reservation plane tempFirst tempLast))
     )
    )
   )
 )
)
 (main-Menu "9")
)
)

(define (search) (and (display "Searching for a reservation")(main-Menu "9")))
(define (edit) (and (display "Editing a reservation")(main-Menu "9")))
(define (cancel) (and (display "Cancelling a reservation")(main-Menu "9")))
(define (quit) (display "Exiting..."))

;; main-menu
(define (main-Menu option)
 (and
  (display "
Created by Christopher Carlisle

========================================
===  Airline seat reservation system ===
========================================

1. Reserve a seat
2. Search
3. Edit a reservation
4. Cancel reservation
5. Quit

Pick an option
")
(cond
  [(string=? option "1") (reserveMenu "9")]
  [(string=? option "2") (search)]
  [(string=? option "3") (edit)]
  [(string=? option "4") (cancel)]
  [(string=? option "5") (quit)]
  [else (main-Menu (read-line))]))
)

 ;;===============================
(main-Menu "6")


