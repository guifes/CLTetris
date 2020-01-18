//
//  formats.h
//  CLTetris
//
//  Created by Guilherme Fernandes on 29/06/19.
//  Copyright © 2019 Guifes. All rights reserved.
//

#ifndef formats_h
#define formats_h

char lineFormat[] =
"....\
 XPXX\
 ....\
 ....\
 .X..\
 .P..\
 .X..\
 .X..";

int lineFormatLen = 16 * 2;

char starFormat[] =
".X..\
 XPX.\
 ....\
 ....\
 .X..\
 .PX.\
 .X..\
 ....\
 ....\
 XPX.\
 .X..\
 ....\
 .X..\
 XP..\
 .X..\
 ....";

int starFormatLen = 16 * 4;

char stepFormat[] =
".XX.\
 XP..\
 ....\
 ....\
 .X..\
 .PX.\
 ..X.\
 ....";

int stepFormatLen = 16 * 2;

char caneRightFormat[] =
"X...\
 XPX.\
 ....\
 ....\
 .XX.\
 .P..\
 .X..\
 ....\
 ....\
 XPX.\
 ..X.\
 ....\
 .X..\
 .P..\
 XX..\
 ....";

int caneRightFormatLen = 16 * 4;

char caneLeftFormat[] =
"....\
 XPX.\
 X...\
 ....\
 XX..\
 .P..\
 .X..\
 ....\
 ..X.\
 XPX.\
 ....\
 ....\
 .X..\
 .P..\
 .XX.\
 ....";

int caneLeftFormatLen = 16 * 4;

char blocFormat[] =
"....\
 .PX.\
 .XX.\
 ....";

int blocFormatLen = 16;

#endif /* formats_h */
