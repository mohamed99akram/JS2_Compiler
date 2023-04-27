Fashel = 10;

print (Fashel);

// ++++++++++++++ if ++++++++++++++
if (Fashel == 2){
    print (Fashel);
}
else{
    print (0);
}

if (Fashel == 10)
    print (Fashel);

// ++++++++++++++ while ++++++++++++++
while(Fashel > 5){
    print (Fashel);
    Fashel = Fashel - 1;
}


// ++++++++++++++ for ++++++++++++++
for (i = 0; i < 5; i = i + 1){
    print (i);
}

for(i = 0; i < 5; i+1){
    print (i);
}


// ++++++++++++++ function ++++++++++++++
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

// ++++++++++++++++ repeat ++++++++++++++
repeat{
    print (Fashel);
}
until(Fashel == 0);

// ++++++++++++++++ enum ++++++++++++++
enum x = 10;
print (x);
enum y;
print (y);

x = 3;
print(x);