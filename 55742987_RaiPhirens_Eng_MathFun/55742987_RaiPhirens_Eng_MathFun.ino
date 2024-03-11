/*
 * Math and English Quiz Fun Game: 
 * Made By Rai Phirens : 55742987
 */
#include <Keypad.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
// Addr, En, Rw, Rs, d4, d5, d6, d7, backlighpin, polarity
// Address could be 0x27, 0x3F, 0x20
LiquidCrystal_I2C lcd(0x20, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //liquidcrystal
byte sad[8] = { //making custom sad face emoji for later use when player loses
  B00000,
  B01010,
  B01010,
  B00000,
  B00000,
  B01110,
  B10001,
};

byte blackBox[] = { //custom blackbox to create animation sequence for loading screen
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

unsigned long time = 0;  //store for time
unsigned long start = 0; //time start
int minutes, minute_unit = 0, minute_digit = 0;             //storing minutes
int seconds, seconds_unit = 0, seconds_digit = 0;           //storing seconds
int milliseconds,milliseconds_unit, milliseconds_digit=0;   //storing milliseconds
byte questions=0;               //questions store

char level;                     //this is where difficulty of the level is stored
boolean typeMode=false;        //this is false because you cannot type when this is false
int number1=0;                
int number2=0;
int number3=0;
int temp=0;
int result;
String operation="";
String sresult;       //to display the result
String sLevel;       //to show the selected level
int large=0;
boolean activate=false;
boolean timer1 = false;   //no timer


char player_figure[4]; //Stores the number of the player

String number_figure = String(); //store the number of the player input
String snumber_figure;   //player input


int bill=0;      //space for answer
int x;
int maximumQuestions=10;     //maximum they will ask 10 questions 
int tries=0;                 //you will start with 0 tries, meaning 0 wrong
int maximumTries=5;          //the maximum wrong you can have is 5

const byte ROWS=4;          //4 rows
const byte COLS=4;          //4 columns
char keys[ROWS][COLS] = {
 {'1','2','3','A'}, 
 {'4','5','6','B'},
 {'7','8','9','C'},
 {'*','0','#','D'}
};
byte rowPins[ROWS] = {13,6,5,4}; //Rows
byte colPins[COLS] = {3,2,1,0}; //Columns
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

int button = 7; //button for lcd backlight dim
int status = false; //button toggle is false

//Verb forms: Past Tense , Present Tense, Past Participle Array
String topQuestions[] = {"1) Present: dug", "2) Past: begin", "3) Past: do", "4) PP: fly", "5) Present: rode",
                         "6) PP: choose", "7) Past: drink", "8) Present: fallen", "9) Past: shake", "10) Past: sing"};   //Questions for T/F
String bottomQuestions[] = {"dig", "begun", "done", "flown", "ride","chosen", "drunk", "fell", "shook", "sang"};         //Answers for T/F
int maxNumQuestions = 9;    //these amount of questions will be asked
int NumQuestions = 0;      //start from question 0
int answers[] = {1, 0, 0, 1, 1, 1, 0, 0, 1, 1};    //1 is correct - 0 is wrong
int score = 0;            //score will be saved here in this integer

void game_over()           //shows this screen when game is over and lost
{
    timer1=false;             //stop the timer from showing
    typeMode=false;            //cannot type answers anymore
    lcd.clear();            
    lcd.setCursor(4,0);
    lcd.print("Game Over");   
    lcd.setCursor(0,1);
    lcd.print("Time: ");
    lcd.print(minute_digit); //displays the minute digit
    lcd.print(minute_unit); // displays the minute unit
    lcd.print(":");
    lcd.print(seconds_digit); //displays the seconds digit
    lcd.print(seconds_unit);  //displays the seconds unit
    lcd.print(":");
    lcd.print(milliseconds_digit); //displays the milliseconds digit
    lcd.print(milliseconds_unit);  //displays the milliseoncds unit
    delay(5000);
    returnMenu();               //after the delay it will go to returnMenu screen
}

void game_win()
{
    timer1=false;             //stop timer from showing
    typeMode=false;           //to stop typing
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Math Game Won!");
    lcd.setCursor(0,1);
    lcd.print("Time: ");
    lcd.print(minute_digit); //displays the minute digit
    lcd.print(minute_unit); // displays the minute unit
    lcd.print(":");
    lcd.print(seconds_digit); //displays the seconds digit
    lcd.print(seconds_unit);  //displays the seconds unit
    lcd.print(":");
    lcd.print(milliseconds_digit); //displays the milliseconds digit
    lcd.print(milliseconds_unit);  //displays the milliseoncds unit
    delay(5000);
    returnMenu();       //return to the returnMenu screen
}

void generate_random()
{
    lcd.clear();
    bill=0;        //reset the answer space to 0, meanning nothing in the answer typed is shown
    sresult="";       //reset the stored answer from question previously
    snumber_figure="";    //reset the input of the player
   
    randomSeed(analogRead(analogRead(0)));     //creat random number
    
    switch(level)  //switch and case for differently difficulty chosen for math game
    {
      case '1':  //easy difficulty 
  
       number1=random(1,11);  //Generates a number between 1 to 10
       number2=random(1,11);  //Generates a number between 1 to 10   
       break;

      case '2':  //medium difficulty
      
       number1=random(50,100);  //Generates a number between 1 to 99
       number2=random(1,11);  //Generates a number between 1 to 10
       break;

      case '3':  //hard difficulty
        
       number1=random(50,100);  //Generates a number between 1 to 99
       number2=random(50,100);  //Generates a number between 1 to 99
       break;

    }    
    
    number3=random(1,5);  //Generates a number between 1 and 4 for operators 
    switch(number3)  //random numbers from 1to 4 is random operator in switch and case
    {
      case 1:  //plus random
        operation="+";
        result=number1+number2;  //random number 1 plus random number 2
        break;
      case 2:   //minus random
        operation="-";
          if(number1<number2) //no negative occurance so choose bigger number
          {
            temp=number1;
            number1=number2;
            number2=temp;
          }
        result=number1-number2; //minus 2 random numbers
        break;
      case 3: //multiply random
        operation="*";
        result=number1*number2;  //multiply 2 random numbers     
        break;
      case 4:
        operation="/"; //divide random
        result=number1%number2;
         if(result!=0)  //If not is zero the mod
         {
         operation="*";
         result=number1*number2;       
         }
         else  //The mod is zero
         {
           result=number1/number2; //divide 2 random numbers
         }
    }
          
    sresult=  String(result);   //change the result to string letters.

    questions = questions + 1;   //question number increase
    lcd.setCursor(0,0);
    lcd.print("Q");
    lcd.print(questions);       //displays the question number
    
    lcd.setCursor(3,0);
    lcd.print(number1);    //shows the numbers 
    lcd.setCursor(5,0);
    lcd.print(operation);
    lcd.setCursor(6,0);
    lcd.print(number2); 
    lcd.setCursor(12,1);
    lcd.print(bill);   
    lcd.setCursor(0,1);  
    lcd.print("    ");    //answer input area
}

void timer()   //timer on screen
{ 
   if(typeMode==true)      //timer starts if this is true
   {
    time = millis()-start;  
    
    minutes = (time/1000)/60; //this calculates the minutes
    minute_unit = minutes % 10; //this calculates the minutes unit
    minute_digit = (minutes - minute_unit)/10; //this calculates the minutes digit

    seconds = (time/1000)%60; //this calculates the seconds
    seconds_unit = seconds % 10;  //this calculates the seconds unit                          
    seconds_digit = (seconds - seconds_unit)/10; //this calculates the seconds digit

    milliseconds = (time/100)%60; //this calculates the milliseconds
    milliseconds_unit = milliseconds % 10; //this calculates the milliseconds unit
    milliseconds_digit = (milliseconds -  milliseconds_unit)/10; //this calculates the milliseconds digit

    lcd.setCursor(8,1);
    lcd.print(minute_digit); //this displays the minutes digit
    lcd.print(minute_unit); //this displays the minutes unit
    lcd.print(":");
    lcd.print(seconds_digit); //this displays the seconds digit
    lcd.print(seconds_unit); //this displays the seconds unit
    lcd.print(":");
    lcd.print(milliseconds_digit); //this displays the milliseconds digit
    lcd.print(milliseconds_unit); //this displays the milliseconds unit    
   }   
}

void setup() 
  {
    // put your setup code here, to run once:
    pinMode(button, INPUT);  //Button for Lcd_backlight
    pinMode(10, OUTPUT); //LED green
    pinMode(9, OUTPUT);  //LED red
    pinMode(8, OUTPUT);  //LED yellow
    lcd.begin(16, 2); 
    lcd.createChar(1,sad);   //create the sad emoji
    lcd.createChar(2,blackBox);  //create the black box
    welcomeMenu();   //opens welcomeMenu
    marioIntro();    //opens marioIntro
    credits();       //opens credits
    lcd.clear();
    delay(100);
    loadingScreen();  //opens loadingscreen
    lcd.clear();   
    menu();    //Displays the select level mode
  
  }
  
//*************************************************************************

void welcomeMenu() //welcome menu
{ 
    delay(100);    
    lcd.setCursor(4,0);
    lcd.print("W");
    delay(100);
    lcd.setCursor(5,0);
    lcd.print("E");
    delay(100);
    lcd.setCursor(6,0);
    lcd.print("L");
    delay(100);
    lcd.setCursor(7,0);
    lcd.print("C");
    delay(100);
    lcd.setCursor(8,0);         //animation in the beginning
    lcd.print("O");
    delay(100);
    lcd.setCursor(9,0);
    lcd.print("M");
    delay(100);
    lcd.setCursor(10,0);
    lcd.print("E");
    delay(100);
    lcd.setCursor(11,0);
    lcd.print("!");
    delay(100);

    lcd.setCursor(0,1);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(1,1);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(2,2);
    lcd.write(byte(2));
    delay(50);  
    lcd.setCursor(3,3);
    lcd.write(byte(2));
    delay(50);          
    lcd.setCursor(4,4);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(5,5);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(6,6);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(7,7);
    lcd.write(byte(2));     //animation of the bar 
    delay(50);
    lcd.setCursor(8,8);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(9,9);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(10,10);
    lcd.write(byte(2));
    delay(50);         
    lcd.setCursor(11,11);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(12,12);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(13,13);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(14,14);
    lcd.write(byte(2));
    delay(50);         
    lcd.setCursor(15,15);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(16,16);
    lcd.write(byte(2));
    delay(50);

    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("To Fun English");
    delay(100);
    lcd.setCursor(1,1);
    lcd.print("and Math Quiz!");
    delay(1000);  
}

void credits(){         //credits 
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Rai Phirens");
    delay(100);
    lcd.setCursor(1,1);
    lcd.print("55742987");
    delay(1000);
}

void loadingScreen()        //loading screen
{ 
    delay(100);
    lcd.setCursor(4,0);
    lcd.print("L");
    delay(100);
    lcd.setCursor(5,0);
    lcd.print("O");
    delay(100);
    lcd.setCursor(6,0);
    lcd.print("A");
    delay(100);
    lcd.setCursor(7,0);
    lcd.print("D");
    delay(100);
    lcd.setCursor(8,0);      //animation loading
    lcd.print("I");
    delay(100);
    lcd.setCursor(9,0);
    lcd.print("N");
    delay(100);
    lcd.setCursor(10,0);
    lcd.print("G");
    delay(100);
    lcd.setCursor(11,0);
    lcd.print("!");
    delay(100);

    lcd.setCursor(0,1);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(1,1);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(2,2);
    lcd.write(byte(2));
    delay(50);  
    lcd.setCursor(3,3);
    lcd.write(byte(2));
    delay(50);          
    lcd.setCursor(4,4);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(5,5);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(6,6);   //animation blackbox
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(7,7);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(8,8);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(9,9);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(10,10);
    lcd.write(byte(2));
    delay(50);         
    lcd.setCursor(11,11);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(12,12);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(13,13);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(14,14);
    lcd.write(byte(2));
    delay(50);         
    lcd.setCursor(15,15);
    lcd.write(byte(2));
    delay(50);
    lcd.setCursor(16,16);
    lcd.write(byte(2));
    delay(50); 
}

void correctAnswer()
{                                   //Function for correct answers
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CORRECT!");
    digitalWrite(10, HIGH);          //turn on Green LED
    tone(11, 2500, 100);             //play tone
    delay(100);
    tone(11, 3500, 100);
    delay(250);
    digitalWrite(10, LOW);           //turn off Green LED
    noTone(11);
    delay(2000);
    lcd.clear();
}


void wrongAnswer()
{                                      //Function for Wrong answers
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WRONG!");            
    digitalWrite(9, HIGH);           //turn on Red LED
    tone(11, 1350, 100);             //play tone
    delay(100);
    tone(11, 1000, 100);
    delay(250);
    digitalWrite(9, LOW);             //turn off Red LED
    noTone(11);
    delay(2000);
    lcd.clear();
}

void pressButton()
{                                   //Function for pressing Button
    tone(11, 1500, 100);
    delay(100);
    tone(11, 2500, 100);        //pressing button creates a tone melody
    delay(250);
    noTone(11);
}

void marioIntro()
{                                 //Mario theme song (short version)
    tone(11, 660, 100);
    delay(150);
    tone(11, 660, 100);
    delay(300);
    tone(11, 660, 100);
    delay(300);
    tone(11, 510, 100);
    delay(100);
    tone(11, 660, 100);
    delay(300);
    tone(11, 770, 100);
    delay(550);
    tone(11, 380, 100);
    delay(575);
}

void returnMenu()         //function for return Menu
{
    timer1=false;           //stop timer
    typeMode=false;         //stop the type mode
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Return To Main");  
    lcd.setCursor(0,1);
    lcd.print("Menu - Press C");
  
}

void showScore()                      //Score on screen for English game
{
    timer1=false;
    typeMode=false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Congratulations!");
    lcd.setCursor(0, 1);
    lcd.print("Your score:");
    lcd.setCursor(13, 1);
    lcd.print(score);
    if (score > 5)                      //LED according to score
    { 
        digitalWrite(10, HIGH);    //green for higher than 5
    }
    else if (score == 5)
    {
        digitalWrite(8, HIGH);     //yellow for exactly 5
    }
    else if (score < 5)
    {
        digitalWrite(9, HIGH);    //red for lower than 5 
    }
    delay(300);
    //end of game
}

void check1()       //checking the math answer
{
    if(snumber_figure==sresult)       //if the answer is correct
    {  
       correctAnswer();       //open correctAnswer function
       generate_random();     //move on to the next generated question
    }
    
    else                      //if the answer is wrong
    {                         
      digitalWrite(9, HIGH);   //Turn red LED
      tone(11, 1350, 100);
      delay(100);                 
      tone(11, 1000, 100);        //tone sound
      delay(250);
      digitalWrite(9, LOW);
      noTone(11);
      delay(2000);
      bill=0;                     //stored space to 0
      snumber_figure="";          //displayed answer to 0
      tries=tries + 1;            //wrong will add one more wrong
    
      
      lcd.setCursor(6,1);
      lcd.write(byte(1));  //this writes the sad face emoji 
    }

  
      lcd.setCursor(8,0);
      lcd.print("Life:");       
      lcd.print(tries);         //will display how many wrongs you have accumulated
      lcd.print("/5");
      
    if(tries==maximumTries)     //if the wrong tries have reached the maximum wrong tries
    {
      game_over();  // Ends the game
    }
    if(questions==maximumQuestions)     //if the questions reach the maximum questions
    {
      game_win();  // Ends the game
    }
}

void menu()       //function for menu
{
    score=0;              //reset score for english to 0
    tries=0;              //reset wrong tries to 0
    questions=0;          //reset questions to 0
    typeMode=false;       //no type mode, cannot type
    digitalWrite(8, LOW);    //turn LED off if open
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Select Game");
    lcd.setCursor(0,1);
    lcd.print("A-English B-Math");

}

void math_game()        //open math difficulty page
{
    typeMode=false;
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Math Level");
    lcd.setCursor(0,1);
    lcd.print("1Easy 2Med 3Hard");

}
void english_gameMenu()      //open english game tutorial
{
    typeMode=false;
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("English Level");
    delay(1000);
    lcd.setCursor(0,1);
    lcd.print("Find the Tenses:");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Past Tense");
    delay(500);
    lcd.setCursor(0,1);
    lcd.print("(Past)");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Present Tense");
    delay(500);
    lcd.setCursor(0,1);
    lcd.print("(Present)");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Past Participle");
    delay(500);
    lcd.setCursor(0,1);
    lcd.print("(PP)");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("True or False");
    delay(500);
    lcd.setCursor(0,1);
    lcd.print("True= * False= #");
    delay(3000);
    lcd.clear();
}

void english_game()                             //function for english game
{  
  lcd.clear();
    if (NumQuestions != maxNumQuestions)            //if the number of question is not equal to maximum number of questions
    {
        lcd.setCursor(0, 0);                        //display question
        lcd.print(topQuestions[NumQuestions]);
        lcd.setCursor(0, 1);                        //display possible answer
        lcd.print(bottomQuestions[NumQuestions]);
    }
   if (NumQuestions == maxNumQuestions)            //5 questions solved? --> score
    {
        showScore();             //score page open
        delay(2000);
        NumQuestions=0;           //reset the questions to 0 
        returnMenu();            //return to the menu page 
    } 
}

void count1()             //countdown before the math game starts 
{
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print(sLevel);      //get the selected level of difficulty
    delay(300);
    lcd.clear();
    for(int x=3;x>=1;x--)   //function for descending numbers 
    {
    lcd.setCursor(8,0);
    lcd.print(x);  //shows 3 2 1 respectively
    delay(300);
    }
    
    lcd.clear();
    lcd.setCursor(7,0);
    lcd.print("Go");       //go display 
    delay(600);


    generate_random();      //random generator after the first go!
      
    typeMode=true;       //allowed to type answers
      
      if(activate==false)  //starts timer and change the active to true
        {
          start=millis();
          activate=true;
        }

}

void loop()
{
      //a) if the button is not pressed the false status is reversed by !status and the LED turns on
      //b) if the button is pressed the true status is reveresed by !status and the LED turns off
      if (digitalRead(button) == true) {    //for button pressed is true
      status = !status;                   //status isnt equal to status for toggling 
      if(status==1){                      //if status is equal to 1
      lcd.backlight();                  //backlight toggles on
      }
      else{
      lcd.noBacklight();                //baclights toggles off the next time when you press the same button
      }
      } while(digitalRead(button) == true);
      delay(50);                  //delay by 50
    timer(); //starts the timer
  
     // put your main code here, to run repeatedly:
  
    char key = keypad.getKey();    //makes it so user can inout in the num pad

  
  
  if(key)        //if a key is inputted
  {   
    //If is the select level display
    if(typeMode==false)    //type mode is false
      {
        if(key=='1' || key=='2' || key=='3')    //press one of these 3 for switch and case
      {
          pressButton();     //press button function for sound
          level=key;      //difficulty is what player inputs
          lcd.clear();      
          lcd.setCursor(5,1);
  
          switch(level)       //swtich for different level difficulty
          {
            case '1':         //press 1
              sLevel="Easy Level";       //make the selected level easy
              break; 
            case '2':          //press 2
              sLevel="Medium Level";      //make the selected level medium
              break;
            case '3':           //press 3
              sLevel="Hard Level";         //make the selected level hard
              break;
          } //end switch
   
          count1();        //go the function count1, so it countdowns after the player presses one of these 3 numbers  

        }

        if(key=='A')       //if press A
        { 
          pressButton();       //function press button for sound 
          english_gameMenu();    //go to english game menu and shows the tutorial on how to play
          english_game();         //finally english game, where you play the game 
        }
        else if(key=='B')        //if press B
        {
          pressButton();         //function press button for sound
          math_game();           //go play the math game 
        }
        if (key=='*')            //if True button (*) is clicked
        {
            if ((answers[NumQuestions]) == 1)       
            {
                correctAnswer(); //play correct answer function
                NumQuestions++;   //next question
                score++;         //add one to score
                english_game();  //open english game 
  
            }
            else if ((answers[NumQuestions]) == 0)
            {
                wrongAnswer();       //play wrong answer function
                NumQuestions++;      //next question
                english_game();      //open english game 
    
            }
        }

        if (key=='#')                 //if False button (#) is clicked
        {
            if ((answers[NumQuestions]) == 0)
            {
                correctAnswer();      //play correct answer function
                NumQuestions++;       //next question
                score++;              //add one to score
                english_game();       //open english game 
            }
            else if ((answers[NumQuestions]) == 1)
            {
                wrongAnswer();        //play wrong answer function
                NumQuestions++;       //next question
                english_game();       //open english game 
            }
        }

        
        
        
        // end IF key select level
          
      } //end IF mode play off
      
      else
      //Mode player

      {
        
           timer1=true;       //changes the timer1 to true 
         
           if(activate==false)   //if active is false, resets the timer
          {
            start=millis();     //resets the timer
            activate=true;      //active change to true
            lcd.clear();
          }        
          
        
        if(key!='A' && key!='B' && key!='C' && key!='D')  //other than these 4, every other numbers in numpad can be pressed
       {
         
        player_figure[bill] = key;   //space no more than 4 nubers 
        
        number_figure = String(key);    //makes the inputted number into string
        
        snumber_figure=snumber_figure+number_figure;    //check input correct or not
      
        lcd.setCursor(0+bill,1);      //cursor according to answer space 
        
        lcd.print(player_figure[bill]);      //pringting the answer
        
        bill++;                       //increase the answer space         
        
       large=sresult.length();        //large is the length of the digits needed for answer
       
       if(bill==large) //check if the answer displayed is larger than the actual space needed, if not, start the checking proccess
        {
        //  for e.g.      lcd.setCursor(10,1);
          check1();    //check the answer is true or not
        }

         
       } //End if key!=   
      
        
      } // End else mode player
      
      
        
    if(key=='C')        //if they input C
        { 
          lcd.clear();
          typeMode=false;       //typeMode will be set to false 
          start=millis();  
          activate=false;      //activate will be set to false 
          pressButton();       //function pressButton for sound 
          loadingScreen();     //open function loading screen
          menu();              //in the end, go back to the menu page to start the game again! 
        }
  }    //End if Key main
} //End loop
