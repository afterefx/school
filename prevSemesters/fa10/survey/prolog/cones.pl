scoop(chocolate).
scoop(vanilla).
scoop(strawberry).
cone(Top, Middle, Bottom):-scoop(Top), scoop(Middle), scoop(Bottom).
