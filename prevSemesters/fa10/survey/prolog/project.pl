%-------- facts -----------
position(mother).
position(father).
position(daughter).
position(son).

gender(father,man).
gender(son,man).
gender(mother,woman).
gender(daughter,woman).

role(witness).
role(victim).
role(helper).
role(murderer).

older(father,mother).
older(mother,son).
older(mother,daughter).


%-------- rules -----------
person(Position, Role):- position(Position), role(Role).
olderThan(X,Z):- older(X,Z).
olderThan(X,Z):- older(X,Y), older(Y,Z).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% check for different genders
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
differentGender(X, Y):-
    person(Xposition, _)=X,
    person(Yposition,_)=Y,
    gender(Xposition,Gender1),
    gender(Yposition,Gender2),
    Gender1 \= Gender2.


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% find out who is possible to
% be the youngest
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
possibleYoungest(V):-
    nextOldest(X),
    person(Y,_)=X,
    older(Y,Z),
    person(Z,_)=V.


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% find youngest
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
youngest(Y):-
    role(R),
    R\=murderer, % based on rule 6
    R\=victim, % based on rule 3
    R\=helper, % based on rule 4
    possibleYoungest(Y),
    oldest(O),
    differentGender(Y,O).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% oldest of the family
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
oldest(V):-
    olderThan(W,X),
    olderThan(W,Y),
    olderThan(W,Z),
    X\=Y,X\=Z,Y\=Z,
    person(W,_)=V,!.


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% nextOldest of the family
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
nextOldest(W):-
    oldest(X),person(Y,_)=X,older(Y,V),person(V,_)=W.


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% the murderer was not the
% youngest member fo the family
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
possibleMurderer(Z):-position(Y),person(Y,_)=Z,not(youngest(Z)).

% the witness and the one who helped the muderer were not of the same sex
witnessHelperDiffGender(X, Y):-
    person(_, witness)=X,
    person(_,helper)=Y,
    differentGender(X,Y).

% the oldest person and the witness were not of the same sex
oldestWitnessDiffGender(Y):-
    oldest(X),person(_,witness)=Y,differentGender(X,Y).


% the youngest person and the victim were not of the same sex
youngestVictimDiffGender(Y):-
    youngest(X),
    person(_,victim)=Y,
    differentGender(X,Y).

% the one who helped the murderer was older than the victim
helperOlderThanVictim(X,Y):-
    person(Xposition, helper)=X,
    person(Yposition, victim)=Y,
    olderThan(Xposition,Yposition).


scenario(D):-
    position(PositionA),
    position(PositionB),
    position(PositionC),
    position(PositionD),
    PositionA \= PositionB,
    PositionA \= PositionC,
    PositionA \= PositionD,
    PositionB \= PositionC,
    PositionB \= PositionD,
    PositionC \= PositionD,
    person(PositionA,witness)=A,
    person(PositionB,victim)=B,
    person(PositionC,helper)=C,
    person(PositionD,murderer)=D,
    witnessHelperDiffGender(A,C),
    oldestWitnessDiffGender(A),
    youngestVictimDiffGender(B),
    helperOlderThanVictim(C,B),
    possibleMurderer(D).


isMurderer(X):-
    scenario(M), person(X, murderer)=M.

%%%%%%%%%%%%%%%%%%%%%
%   w h v m
% m       x
% f   x
% d x
% s     x

