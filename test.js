print("Hello World");
Fashel = 10;

print (Fashel);
// ++++++++++++++ const ++++++++++++++
print("CONST");
const x = 10;
print (x);

// ++++++++++++++ if ++++++++++++++
print("IF");
if (Fashel == 2){
    print (Fashel);
}
else{
    print (0);
}

if (Fashel == 10)
    print (Fashel);

if (Fashel == 5){
    print (Fashel);
}
else if (Fashel == 10){
    print (20);
}


// ++++++++++++++ while ++++++++++++++
print("WHILE");
while(Fashel > 5){
    print (Fashel);
    Fashel = Fashel - 1;
}


// ++++++++++++++ for ++++++++++++++
print("FOR");
for (i = 0; i < 5; i = i + 1){
    print (i);
}

// ++++++++++++++ function ++++++++++++++
print("FUNCTION");
function test(){
    print (Fashel);
}
test();
test();

function test2(x){
    print (x);
}

test2(Fashel);

test2(10);

test2(10, 20, 40);

// ++++++++++++++++ do ++++++++++++++
// Fashel = 10;
print("DO");
do {
    print (Fashel);
    Fashel = Fashel - 1;
} while (Fashel > 5);

// ++++++++++++++++ enum ++++++++++++++
print("ENUM");
{A, B, C};

// print (A);
// print (B);
// print (C);

// ++++++++++++++++ switch ++++++++++++++
print("SWITCH");
switch (x){
    case 1:
        {
        print (1);
        print(1);
        }
    case 2:
        print (2);
    case 3:
        print (3);
    default:
        print (0);
}

switch (x){
    default:
        print (0);
}

switch (x){
    case 1:
        print (1);
}

print("DONE");