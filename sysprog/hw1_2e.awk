BEGIN { print ""; print "    ------Home Runs Data:------";print ""}
{ if($2==155) pf+=1;}
{ if($2==158) pe+=1;}
{ if($2==161) po+=1;}

{ if($2==155) rf+=$4;}
{ if($2==158) re+=$4;}
{ if($2==161) ro+=$4;}

END {print "Players who played 155 games:",pf;
     print "Total Home Runs:             ", rf;
     print "";
     print "Players who played 158 games:",pe;
     print "Total Home Runs:             ",re;
     print "";
     print "Players who played 161 games:",po;
     print "Total Home Runs:             ",ro;
     print "";}
