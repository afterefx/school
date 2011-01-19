int thirdCoordinate(double * a, double *b, int n)
{
double r[2],s[2],t[2],a[2],b[2];
/*cout << "========= A =========\n";
cout << "Enter x coordinate: ";
cin >> a[0];
cout << "Enter y coordinate: ";
cin >> a[1];
cout << "========= B =========\n";
cout << "Enter x coordinate: ";
cin >> b[0];
cout << "Enter y coordinate: ";
cin >> b[1];*/

double mid[2];
double xdelta, ydelta;
const double factor = 0.288675135;

r[0]=(2*a[0]+b[0])/3.0;
r[1]=(2*a[1]+b[1])/3.0;
t[0]=(a[0]+2*b[0])/3.0;
t[1]=(a[1]+2*b[1])/3.0;
mid[0]=(a[0]+b[0])/2.0;
mid[1]=(a[1]+b[1])/2.0;


xdelta = b[0]-a[0];
ydelta = b[1]-a[1];

s[0]=mid[0]+ factor * ydelta;
s[1]=mid[1]- factor * xdelta;

cout << "Your coordinates are " ;
cout << s[0] ;
cout << " for x " ;
cout << s[1] << " for y.\n";

}

