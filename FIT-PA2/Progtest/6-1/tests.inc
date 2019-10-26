bool st;
int  cnt;

// CZone z0 ( "fit.cvut.cz" );
// st = z0 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ); // st = true
// cout << boolalpha << st << endl;
// st = z0 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:0:1:2:3" ) ) ); // st = true
// cout << boolalpha << st << endl;
// st = z0 . Add ( CRecA ( "edux", CIPv4 ( "147.32.232.158" ) ) ); // st = true
// cout << boolalpha << st << endl;
// st = z0 . Add ( CRecA ( "edux", CIPv4 ( "147.32.232.160" ) ) ); // st = true
// cout << boolalpha << st << endl;
// st = z0 . Add ( CRecA ( "edux", CIPv4 ( "147.32.232.159" ) ) ); // st = true
// cout << boolalpha << st << endl;
// st = z0 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:1:2:3:4" ) ) ); // st = true
// cout << boolalpha << st << endl;
// st = z0 . Add ( CRecMX ( "edux", "relay.fit.cvut.cz.", 0 ) ); // st = true
// cout << boolalpha << st << endl;
// st = z0 . Add ( CRecMX ( "edux", "relay2.fit.cvut.cz.", 10 ) ); // st = true
// cout << boolalpha << st << endl;
// cout << z0;
/*
--8<----8<----8<----8<----8<----8<----8<--
fit.cvut.cz
 +- progtest A 147.32.232.142
 +- progtest AAAA 2001:718:2:2902:0:1:2:3
 +- edux A 147.32.232.158
 +- edux A 147.32.232.160
 +- edux A 147.32.232.159
 +- progtest AAAA 2001:718:2:2902:1:2:3:4
 +- edux MX 0 relay.fit.cvut.cz.
 \- edux MX 10 relay2.fit.cvut.cz.
--8<----8<----8<----8<----8<----8<----8<--
*/
// cnt = z0 . Search ( "progtest" ) . Count (); // cnt = 3
// cout << cnt << endl;
// cout << z0 . Search ( "progtest" );
// /*
// --8<----8<----8<----8<----8<----8<----8<--
// progtest A 147.32.232.142
// progtest AAAA 2001:718:2:2902:0:1:2:3
// progtest AAAA 2001:718:2:2902:1:2:3:4
// --8<----8<----8<----8<----8<----8<----8<--
// */
// st = z0 . Del ( CRecA ( "edux", CIPv4 ( "147.32.232.160" ) ) ); // st = true
// cout << boolalpha << st << endl;
// st = z0 . Add ( CRecA ( "edux", CIPv4 ( "147.32.232.122" ) ) ); // st = true
// cout << z0;
// /*
// --8<----8<----8<----8<----8<----8<----8<--
// fit.cvut.cz
//  +- progtest A 147.32.232.142
//  +- progtest AAAA 2001:718:2:2902:0:1:2:3
//  +- edux A 147.32.232.158
//  +- edux A 147.32.232.159
//  +- progtest AAAA 2001:718:2:2902:1:2:3:4
//  +- edux MX 0 relay.fit.cvut.cz.
//  +- edux MX 10 relay2.fit.cvut.cz.
//  \- edux A 147.32.232.122
// --8<----8<----8<----8<----8<----8<----8<--
// */
// cnt = z0 . Search ( "edux" ) . Count (); // cnt = 5
// cout << cnt << endl;
// cout << z0 . Search ( "edux" );
// /*
// --8<----8<----8<----8<----8<----8<----8<--
// edux A 147.32.232.158
// edux A 147.32.232.159
// edux MX 0 relay.fit.cvut.cz.
// edux MX 10 relay2.fit.cvut.cz.
// edux A 147.32.232.122
// --8<----8<----8<----8<----8<----8<----8<--
// */
// cout << z0 . Search ( "edux" ) [ 0 ];
//  // edux A 147.32.232.158
// cout << z0  . Search ( "edux" ) [ 0 ] . Name () << endl;
//  // edux
// cout << z0 . Search ( "edux" ) [ 0 ] . Type () << endl;
//  // A
// cout << z0 . Search ( "edux" ) [ 1 ];
//  // edux A 147.32.232.159
// cout << z0  . Search ( "edux" ) [ 1 ] . Name () << endl;
//  // edux
// cout << z0 . Search ( "edux" ) [ 1 ] . Type () << endl;
//  // A
// cout << z0 . Search ( "edux" ) [ 2 ];
//  // edux MX 0 relay.fit.cvut.cz.
// cout << z0  . Search ( "edux" ) [ 2 ] . Name () << endl;
//  // edux
// cout << z0 . Search ( "edux" ) [ 2 ] . Type () << endl;
//  // MX
// cout << z0 . Search ( "edux" ) [ 10 ];
//  // exception thrown


// CZone z2 ( "fit.cvut.cz" );
// st = z2 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ); // st = true
// st = z2 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:0:1:2:3" ) ) ); // st = true
// st = z2 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.144" ) ) ); // st = true
// st = z2 . Add ( CRecMX ( "progtest", "relay.fit.cvut.cz.", 10 ) ); // st = true
// st = z2 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ); // st = false
// st = z2 . Del ( CRecA ( "progtest", CIPv4 ( "147.32.232.140" ) ) ); // st = false
// st = z2 . Del ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ); // st = true
// st = z2 . Del ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ); // st = false
// st = z2 . Add ( CRecMX ( "progtest", "relay.fit.cvut.cz.", 20 ) ); // st = true
// st = z2 . Add ( CRecMX ( "progtest", "relay.fit.cvut.cz.", 10 ) ); // st = false
// cout << z2;

// --8<----8<----8<----8<----8<----8<----8<--
// fit.cvut.cz
//  +- progtest AAAA 2001:718:2:2902:0:1:2:3
//  +- progtest A 147.32.232.144
//  +- progtest MX 10 relay.fit.cvut.cz.
//  \- progtest MX 20 relay.fit.cvut.cz.
// --8<----8<----8<----8<----8<----8<----8<--

// cnt = z2 . Search ( "progtest" ) . Count (); // cnt = 4
// cout << cnt << endl;
// cout << z2 . Search ( "progtest" );
// /*
// --8<----8<----8<----8<----8<----8<----8<--
// progtest AAAA 2001:718:2:2902:0:1:2:3
// progtest A 147.32.232.144
// progtest MX 10 relay.fit.cvut.cz.
// progtest MX 20 relay.fit.cvut.cz.
// --8<----8<----8<----8<----8<----8<----8<--
// */
// cnt = z2 . Search ( "edux" ) . Count (); // cnt = 0
// cout << cnt << endl;
// cout << z2 . Search ( "edux" );
// /*
// --8<----8<----8<----8<----8<----8<----8<--
// --8<----8<----8<----8<----8<----8<----8<--
// */

CZone z4 ( "fit.cvut.cz" );
st = z4 . Add ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ); // st = true
st = z4 . Add ( CRecA ( "edux", CIPv4 ( "147.32.232.158" ) ) ); // st = true
st = z4 . Add ( CRecA ( "edux", CIPv4 ( "147.32.232.160" ) ) ); // st = true
st = z4 . Add ( CRecA ( "edux", CIPv4 ( "147.32.232.159" ) ) ); // st = true
CZone z5 ( z4 );
st = z4 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:0:1:2:3" ) ) ); // st = true
st = z4 . Add ( CRecAAAA ( "progtest", CIPv6 ( "2001:718:2:2902:1:2:3:4" ) ) ); // st = true
st = z5 . Del ( CRecA ( "progtest", CIPv4 ( "147.32.232.142" ) ) ); // st = true
cout << z4;
// /*
// --8<----8<----8<----8<----8<----8<----8<--
// fit.cvut.cz
//  +- progtest A 147.32.232.142
//  +- edux A 147.32.232.158
//  +- edux A 147.32.232.160
//  +- edux A 147.32.232.159
//  +- progtest AAAA 2001:718:2:2902:0:1:2:3
//  \- progtest AAAA 2001:718:2:2902:1:2:3:4
// --8<----8<----8<----8<----8<----8<----8<--
// */
cout << z5;
// /*
// --8<----8<----8<----8<----8<----8<----8<--
// fit.cvut.cz
//  +- edux A 147.32.232.158
//  +- edux A 147.32.232.160
//  \- edux A 147.32.232.159
// --8<----8<----8<----8<----8<----8<----8<--
// */
z5 = z4;
st = z4 . Add ( CRecMX ( "edux", "relay.fit.cvut.cz.", 0 ) ); // st = true
st = z4 . Add ( CRecMX ( "edux", "relay2.fit.cvut.cz.", 10 ) ); // st = true
cout << z4;
// /*
// --8<----8<----8<----8<----8<----8<----8<--
// fit.cvut.cz
//  +- progtest A 147.32.232.142
//  +- edux A 147.32.232.158
//  +- edux A 147.32.232.160
//  +- edux A 147.32.232.159
//  +- progtest AAAA 2001:718:2:2902:0:1:2:3
//  +- progtest AAAA 2001:718:2:2902:1:2:3:4
//  +- edux MX 0 relay.fit.cvut.cz.
//  \- edux MX 10 relay2.fit.cvut.cz.
// --8<----8<----8<----8<----8<----8<----8<--
// */
cout << z5;
/*
// --8<----8<----8<----8<----8<----8<----8<--
// fit.cvut.cz
//  +- progtest A 147.32.232.142
//  +- edux A 147.32.232.158
//  +- edux A 147.32.232.160
//  +- edux A 147.32.232.159
//  +- progtest AAAA 2001:718:2:2902:0:1:2:3
//  \- progtest AAAA 2001:718:2:2902:1:2:3:4
// --8<----8<----8<----8<----8<----8<----8<--
// */

