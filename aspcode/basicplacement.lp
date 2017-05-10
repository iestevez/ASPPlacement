wall(a;b;c;d).

%  All wall lengths are equal. 
dimroom((h;v),16).

% Wall restricted areas:
% maxheight/4: wall, start, end, max height
%maxheight(a,2,5,3).
%maxheight(a,7,10,0).
%maxheight(c,2,5,3).
%maxheight(c,7,10,0).

% We have also objects.
object(1..12).

% Object dimension (all of them are cubes):
% size/4: object id, hor. length, ver length. height
size(1,2,6,1).
size(2,2,6,1).
size(3,2,6,1).
size(4,2,6,1).
size(5,2,6,1).
size(6,2,6,1).
size(7,2,6,1).
size(8,2,6,1).
size(9,2,6,1).
size(10,2,6,1).
size(11,2,6,1).
size(12,2,6,1).

% Object sides that shoud be oriented towards the wall.
% towall/2: ide object, side
towall(1,a).
towall(2,a).
towall(3,a).
towall(4,a).
towall(5,a).
towall(6,a).
towall(7,a).
towall(8,a).
towall(9,a).
towall(10,a).
towall(11,a).
towall(12,a).

% --------------------------------------------------------
% Second section: problem definition (auxiliary, generation and constraints).
% --------------------------------------------------------
% Auxiliary definitions 
% --------------------------------------------------------

% Auxiliary: object effective length
% Auxiliary: length of each wall.
lenw((a;c),L):-dimroom(h,L).
lenw((b;d),L):-W=(b;d),dimroom(v,L).
numobjects(X):-X=#max{O:object(O)}.

% We define rotations using the same names as walls
% rotation(W) means side a goes to occupy W orientation. Therefore rotation(a) means no rotation.
rotation(W):-wall(W).

% We define the rotation operator oprot(side, number of 90º rots counter clock-wise, new side at the position of the original side)
% This is equivalent to oprot(W,n,W1). If n=0 side a is towards wall
% If n=1 then side b is towards wall.
% If n=2 then side c is towards wall.
% If n=3 then side d is towards wall.
oprot(a,0,a). oprot(a,1,b). oprot(a,2,c). oprot(a,3,d).
oprot(b,0,b). oprot(b,1,c). oprot(b,2,d). oprot(b,3,a).
oprot(c,0,c). oprot(c,1,d). oprot(c,2,a). oprot(c,3,b).
oprot(d,0,d). oprot(d,1,a). oprot(d,2,b). oprot(d,3,c).

% Valid lengths for an object are dimx or dimy
validlong(O,L):-object(O),size(O,L,_,_).
validlong(O,L):-object(O),size(O,_,L,_). 
% Horizontal dimension is dimx
hordim(O,L):-size(O,L,_,_).
% Vertical dimension is dimy
verdim(O,L):-size(O,_,L,_).
% Dimension of a particular face in an object
dimface(O,F,L):-F=(a;c),hordim(O,L).
dimface(O,F,L):-F=(b;d),verdim(O,L).

% Effective area is the projection of object at the floor. 
% effectivearea/5. 1. Object 2,3. X1,Y1 coordinates. 4,5. X2,Y2 coordinates
% Depends on object dimensions and object placement.
% Notice that placement is defined as placement/4where 1. Object, 2. Wall, 3. Position at the wall. 4. Rotation.
effectivearea(O,X1,Y1,X2,Y2):-object(O),size(O,L,H,_),placement(O,a,OP,R),X1=OP,X2=(OP+L),Y1=0,Y2=H,#false:R=(b;d).
effectivearea(O,X1,Y1,X2,Y2):-object(O),size(O,L,H,_),placement(O,a,OP,R),X1=OP,X2=(OP+H),Y1=0,Y2=L,#false:R=(a;c).
effectivearea(O,X1,Y1,X2,Y2):-object(O),size(O,L,H,_),placement(O,b,OP,R),lenw(a,W),X1=W-L,X2=W,Y1=OP,Y2=OP+H,#false:R=(b;d).
effectivearea(O,X1,Y1,X2,Y2):-object(O),size(O,L,H,_),placement(O,b,OP,R),lenw(a,W),X1=W-H,X2=W,Y1=OP,Y2=OP+L,#false:R=(a;c).
effectivearea(O,X1,Y1,X2,Y2):-object(O),size(O,L,H,_),placement(O,c,OP,R),lenw(a,Wa),lenw(b,Wb),X1=Wa-(OP+L),X2=(Wa-OP),Y1=Wb-H,Y2=Wb,#false:R=(b;d).
effectivearea(O,X1,Y1,X2,Y2):-object(O),size(O,L,H,_),placement(O,c,OP,R),lenw(a,Wa),lenw(b,Wb),X1=Wa-(OP+H),X2=(Wa-OP),Y1=Wb-L,Y2=Wb,#false:R=(a;c).
effectivearea(O,X1,Y1,X2,Y2):-object(O),size(O,L,H,_),placement(O,d,OP,R),lenw(a,W),X1=0,X2=L,Y1=OP,Y2=(OP+H),#false:R=(b;d).
effectivearea(O,X1,Y1,X2,Y2):-object(O),size(O,L,H,_),placement(O,d,OP,R),lenw(a,W),X1=0,X2=H,Y1=OP,Y2=(OP+L),#false:R=(a;c).

% fitroom/1. Determines if object O at fitroom(O) fits at room. A placement is assumed for object O.
fitroom(O):-object(O),effectivearea(O,X1,Y1,X2,Y2),X1>=0,Y1>=0, lenw(a,La),lenw(b,Lb),X2<=La,Y2<=Lb.

% Determining overlaping
% ----------------------

% Simple overlapping.
inrange(X):-X=0..L,dimroom(h,L).
inrange(X):-X=0..L,dimroom(v,L).
overlap1d(S1,E1,S2,E2):-inrange(S1),inrange(E1),inrange(S2),inrange(E2),S2>=S1,S2<=E1.
overlap1d(S1,E1,S2,E2):-inrange(S1),inrange(E1),inrange(S2),inrange(E2),S1>=S2,S1<=E2.

% overlaphor(O1,O2) determines if there is an overlap in the horizontal dimensions between object O1 and object O2.
overlaphor(O1,O2):-effectivearea(O1,X11,_,X12,_),effectivearea(O2,X21,_,X22,_),X12>=X21,X12<=X22.
overlaphor(O1,O2):-effectivearea(O1,X11,_,X12,_),effectivearea(O2,X21,_,X22,_),X22>=X11,X22<=X12.
% overlapver(O1,O2) is the same as overlaphor(O1,O2) but in the vertical dimension.
overlapver(O1,O2):-effectivearea(O1,_,Y11,_,Y12),effectivearea(O2,_,Y21,_,Y22),Y12>=Y21,Y12<=Y22.
overlapver(O1,O2):-effectivearea(O1,_,Y11,_,Y12),effectivearea(O2,_,Y21,_,Y22),Y22>=Y11,Y22<=Y12.
% overlap(O1,O2) determines an overlap betweeb object O1 and object O2.
overlap(O1,O2):-O1!=O2,overlaphor(O1,O2),overlapver(O1,O2).
% ----------------------

% Determining which side of othe object is towards the wall.
% ----------------------
% facetowall(O,a) is true is object O has its "a" side towards wall.
facetowall(O,a):-placement(O,W,_,W1),wall(W),oprot(W,0,W1).
facetowall(O,b):-placement(O,W,_,W1),wall(W),oprot(W,3,W1).
facetowall(O,c):-placement(O,W,_,W1),wall(W),oprot(W,2,W1).
facetowall(O,d):-placement(O,W,_,W1),wall(W),oprot(W,1,W1).
% ----------------------

% Determining if object fit at wall
% --------------------------------
fitwall(O):-facetowall(O,F),placement(O,W,OP,_),lenw(W,L),dimface(O,F,LF),(OP+LF)<=L.
% --------------------------------

% ------------------------------------------------
% Generative part of model.
% ------------------------------------------------

% For each object we need one placement.
% placement/4. 1. Object, 2. Wall, 3. Position at wall, 4. Rotation (a,b,c,d).
{placement(O,W,OP,R):wall(W),lenw(W,L),OP=0..L,rotation(R)}<=1:-object(O).

% ---------------------------------------------------
% Constraints.
% ----------------------------------------------------

% If towall(O,W) then facetowall(O,W)
:-object(O),wall(W),placement(O,_,_,_),towall(O,W),not facetowall(O,W).
% Every object placement should make the object to fit at room
:-object(O),placement(O,_,_,_),not fitroom(O).
% Avoid overlaping of any two objects.
:-object(O1),object(O2), placement(O1,_,_,_), placement(O2,_,_,_),overlap(O1,O2).
% Use maxheight constraints
:-object(O),facetowall(O,F),placement(O,W,OP,_),maxheight(W,S,E,H),dimface(O,F,LF), size(O,_,_,HO),OP>=S,OP>=E,HO>O.
:-object(O),facetowall(O,F),placement(O,W,OP,_),maxheight(W,S,E,H),dimface(O,F,LF), size(O,_,_,HO),LO=OP+LF,S>=OP,S<=LO,HO>O.


% Weak constraint.
% ----------------
% Try to place as many objects as possible.
#maximize{1@1,O: placement(O,_,_,_)}.
% -----------------
%:-placement(O,_,_,_). [ -1@1,O ]
%:-object(O),wall(W),rotation(R),lenw(W,L),not placement(O,W,OP,R).
%:-#count{placement(O,W,OP,R):object(O),wall(W),lenw(W,L),OP=0..L,rotation(R)}!=1.

#show numobjects/1.
%#show effectivearea/5.
#show placement/4.
