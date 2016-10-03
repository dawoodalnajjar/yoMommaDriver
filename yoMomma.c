/* Hi people, this was my first driver. I used Derek Molloy's ebbchar driver as base found here
 * http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
 * Many thanks to Derek Molloy on his tutorials. They were a great start for me to start developing
 * Linux drivers.
 * This a linux Driver that tells yo Momma jokes :) I know, it's rediculous, but it is also funny!
 * Use it at your own risk!
 */

#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <asm/uaccess.h>          // Required for the copy to user function
#define  DEVICE_NAME "yoMomma"    ///< The device will appear at /dev/yoMomma using this value
#define  CLASS_NAME  "yoMommaClass"        ///< The device class -- this is a character device driver
 

 
static int    majorNumber;                  ///< Stores the device number -- determined automatically
static int    numberOpens = 0;              ///< Counts the number of times the device is opened
static struct class*  yoMommaClass  = NULL; ///< The device-driver class struct pointer
static struct device* yoMommaDevice = NULL; ///< The device-driver device struct pointer
const char * yoMommas[744];
// The prototype functions for the character driver -- must come before the struct definition
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

/** @brief Devices are represented as file structure in the kernel. The file_operations structure from
 *  /linux/fs.h lists the callback functions that you wish to associated with your file operations
 *  using a C99 syntax structure. char devices usually implement open, read, write and release calls
 */
static struct file_operations fops =
{
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release,
};
 
 
 
/** @brief The LKM initialization function
 *  The static keyword restricts the visibility of the function to within this C file. The __init
 *  macro means that for a built-in driver (not a LKM) the function is only used at initialization
 *  time and that it can be discarded and its memory freed up after that point.
 *  @return returns 0 if successful
 */
static int __init yoMomma_init(void){
	printk(KERN_INFO "yoMomma: Initializing the yoMomma LKM\n");
 
	// Try to dynamically allocate a major number for the device -- more difficult but worth it
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber<0){
		printk(KERN_ALERT "yoMomma failed to register a major number\n");
		return majorNumber;
	}
	printk(KERN_INFO "yoMomma: registered correctly with major number %d\n", majorNumber);
 
	// Register the device class
	yoMommaClass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(yoMommaClass)){                // Check for error and clean up if there is
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device class\n");
		return PTR_ERR(yoMommaClass);          // Correct way to return an error on a pointer
	}
	printk(KERN_INFO "yoMomma: device class registered correctly\n");
 
	// Register the device driver
	yoMommaDevice = device_create(yoMommaClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if (IS_ERR(yoMommaDevice)){               // Clean up if there is an error
		class_destroy(yoMommaClass);           // Repeated code but the alternative is goto statements
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create the device\n");
		return PTR_ERR(yoMommaDevice);
	}
	printk(KERN_INFO "yoMomma: device class created correctly\n"); // Made it! device was initialized
	
	yoMommas[0]="Yo momma so fat were in her right now\n";
	yoMommas[1]="Yo momma so fat people jog around her for exercise\n";
	yoMommas[2]="Yo momma so fat she went to the movies and sat next to everyone\n";
	yoMommas[3]="Yo momma so fat she has been declared a natural habitat for Condors\n";
	yoMommas[4]="Yo mamma so fat you haveta roll over twice to get off her...\n";
	yoMommas[5]="Yo momma so fat she was floating in the ocean and spain claimed her for then new world\n";
	yoMommas[6]="Yo momma so fat she lay on the beach and people run around yelling Free Willy\n";
	yoMommas[7]="Yo momma so fat when you get on top of her your ears pop!\n";
	yoMommas[8]="Yo momma so fat when she has sex, she has to give directions!\n";
	yoMommas[9]="Yo momma so fat she goes to a resturant, looks at the menu and says 'okay!'\n";
	yoMommas[10]="Yo momma so fat when she wears a yellow raincoat, people said 'Taxi!'\n";
	yoMommas[11]="Yo momma so fat she had to go to Sea World to get baptized\n";
	yoMommas[12]="Yo momma so fat she got to iron her pants on the driveway\n";
	yoMommas[13]="Yo momma so fat she put on her lipstick with a paint-roller\n";
	yoMommas[14]="Yo momma so fat she got to pull down her pants to get into her pockets\n";
	yoMommas[15]="Yo momma so fat when she tripped over on 4th Ave, she landed on 12th\n";
	yoMommas[16]="Yo momma so fat when she bungee jumps, she brings down the bridge too\n";
	yoMommas[17]="Yo momma so fat the highway patrol made her wear 'Caution! Wide Turn'\n";
	yoMommas[18]="Yo momma so fat when she sits around the house, she SITS AROUND THE HOUSE!\n";
	yoMommas[19]="Yo momma so fat when she steps on a scale, it read 'one at a time, please'\n";
	yoMommas[20]="Yo momma so fat when she sits on my face I can't hear the stereo.\n";
	yoMommas[21]="Yo momma so fat she fell in love and broke it.\n";
	yoMommas[22]="Yo momma so fat when she gets on the scale it says to be continued.\n";
	yoMommas[23]="Yo momma so fat when she gets on the scale it says we don't do livestock.\n";
	yoMommas[24]="Yo momma so fat whenever she goes to the beach the tide comes in!\n";
	yoMommas[25]="Yo momma so fat when she plays hopscotch, she goes New York, L.A.,  Chicago...\n";
	yoMommas[26]="Yo momma so fat she's got Amtrak written on her leg.\n";
	yoMommas[27]="Yo momma so fat even Bill Gates couldn't pay for her liposuction!\n";
	yoMommas[28]="Yo momma so fat her legs is like spoiled milk - white & chunky!\n";
	yoMommas[29]="Yo momma so fat you have to roll her ass in flour and look for the wet spot to fuck her!\n";
	yoMommas[30]="Yo momma so fat I had to take a train and two buses just to get on the bitches good side!\n";
	yoMommas[31]="Yo momma so fat she wakes up in sections!\n";
	yoMommas[32]="Yo momma so fat when she goes to an amusement park, people try to ride HER!\n";
	yoMommas[33]="Yo momma so fat she sat on a quarter and a booger shot out of george washington's nose.\n";
	yoMommas[34]="Yo momma so fat she was mistaken for God's bowling ball!\n";
	yoMommas[35]="Yo momma so fat she rolled over 4 quarters and it made a dollar!\n";
	yoMommas[36]="Yo momma so fat when she lies on the beach no one else gets sun!\n";
	yoMommas[37]="Yo momma so fat when she bunje jumps she goes straight to hell!\n";
	yoMommas[38]="Yo momma so fat when she jumps up in the air she gets stuck!!!\n";
	yoMommas[39]="Yo momma so fat she's got more Chins than a Hong Kong phone book!\n";
	yoMommas[40]="Yo momma so fat that her senior pictures had to be arial views!\n";
	yoMommas[41]="Yo momma so fat she's on both sides of the family!\n";
	yoMommas[42]="Yo momma so fat everytime she walks in high heels, she strikes oil!\n";
	yoMommas[43]="Yo momma so fat she fell and made the Grand Canyon!\n";
	yoMommas[44]="Yo momma so fat she sat on the beach and Greenpeace threw her in!\n";
	yoMommas[45]="Yo momma so fat the animals at the zoo feed her.\n";
	yoMommas[46]="Yo momma so fat she was baptized at Marine World.\n";
	yoMommas[47]="Yo momma so fat she's on both sides of the family!\n";
	yoMommas[48]="Yo momma so fat when she dances at a concert the whole band skips.\n";
	yoMommas[49]="Yo momma so fat the Aids quilt wouldn't cover her\n";
	yoMommas[50]="Yo momma so fat she stands in two time zones.\n";
	yoMommas[51]="Yo momma so fat I tried to drive around her and I ran out of gas.\n";
	yoMommas[52]="Yo momma so fat she left the house in high heels and when she came back she had on flip flops.\n";
	yoMommas[53]="Yo momma so fat shes on both sides of the family.\n";
	yoMommas[54]="Yo momma so fat it takes her two trips to haul ass\n";
	yoMommas[55]="Yo momma so fat you have to grease the door frame and hold a twinkie on the other side just to get her through\n";
	yoMommas[56]="Yo momma so fat when the bitch goes to an all you can eat buffet, they have to install speed bumps.\n";
	yoMommas[57]="Yo momma so fat that she cant tie her own shoes.\n";
	yoMommas[58]="Yo momma so fat sets off car alarms when she runs.\n";
	yoMommas[59]="Yo momma so fat she cant reach her back pocket.\n";
	yoMommas[60]="Yo momma so fat when she wears one of those X jackets, helicopters try to land on her back!\n";
	yoMommas[61]="Yo momma so fat her college graduation picture was an airial.\n";
	yoMommas[62]="Yo momma so fat she broke her leg, and gravy poured out!\n";
	yoMommas[63]="Yo momma so fat when she rides in a hot air balloon, it looks like she's wearin tights!\n";
	yoMommas[64]="Yo momma so fat she got hit by a parked car! \n";
	yoMommas[65]="Yo momma so fat they have to grease the bath tub to get her out!\n";
	yoMommas[66]="Yo momma so fat she has a run in her blue-jeans!\n";
	yoMommas[67]="Yo momma so fat when she gets on the scale it says to be continued.\n";
	yoMommas[68]="Yo momma so fat when she wears a yellow raincoat people say 'Taxi!'\n";
	yoMommas[69]="Yo momma so fat she got to iron her pants on the driveway!\n";
	yoMommas[70]="Yo momma so fat she put on her lipstick with a paint-roller!\n";
	yoMommas[71]="Yo momma so fat when she tripped over on 4th Ave she landed on 12th\n";
	yoMommas[72]="Yo momma so fat when she bungee jumps she pulls down the bridge too\n";
	yoMommas[73]="Yo momma so fat she steps on a scale & it goes one at a time please\n";
	yoMommas[74]="Yo momma so fat she fell in love and broke it!\n";
	yoMommas[75]="Yo momma so fat she jumped up in the air and got stuck!\n";
	yoMommas[76]="Yo momma so fat she fell in love and broke it.\n";
	yoMommas[77]="Yo momma so fat when she sits on my face I can't hear the stereo.\n";
	yoMommas[78]="Yo momma so fat she eats Wheat Thicks.\n";
	yoMommas[79]="Yo momma so fat her neck looks like a pair of hot dogs!\n";
	yoMommas[80]="Yo momma so fat she's got her own area code!\n";
	yoMommas[81]="Yo momma so fat she looks like she's smuggling a Volkswagon!\n";
	yoMommas[82]="Yo momma so fat God couldn't light Earth till she moved!\n";
	yoMommas[83]="Yo momma so fat NASA has to orbit a satellite around her!\n";
	yoMommas[84]="Yo momma so fat that when she hauls ass, she has to make two trips!\n";
	yoMommas[85]="Yo momma so fat even her clothes have stretch marks!\n";
	yoMommas[86]="Yo momma so fat she has a wooden leg with a kickstand!\n";
	yoMommas[87]="Yo momma so fat she has to use a VCR as a beeper!\n";
	yoMommas[88]="Yo momma so fat they use the elastic in her underwear for bungee jumping\n";
	yoMommas[89]="Yo momma so fat when they used her underwear elastic for bungee jumping, they hit the ground.\n";
	yoMommas[90]="Yo momma so fat when she back up she beep.\n";
	yoMommas[91]="Yo momma so fat she jumped up in the air and got stuck.\n";
	yoMommas[92]="Yo momma so fat she has to buy two airline tickets.\n";
	yoMommas[93]="Yo momma so fat when she fell over she rocked herself asleep trying to get up again.\n";
	yoMommas[94]="Yo momma so fat she influences the tides.\n";
	yoMommas[95]="Yo momma so fat that when I tried to drive around her I ran out of gas.\n";
	yoMommas[96]="Yo momma so fat she broke her leg and gravy fell out.\n";
	yoMommas[97]="Yo momma so fat she lays on the beach and greenpeace tried to push her back in the water\n";
	yoMommas[98]="Yo momma so fat she broke her leg and gravy poured out\n";
	yoMommas[99]="Yo momma so fat she uses redwoods to pick her teeth\n";
	yoMommas[100]="Yo momma so fat the only pictures you have of her are satellite pictures\n";
	yoMommas[101]="Yo momma so fat she jumped in the air and got stuck.\n";
	yoMommas[102]="Yo momma so fat she put on some BVD's and by the time they reached her waist they spelled out boulevard.\n";
	yoMommas[103]="Yo momma so fat she sat on a dollar and squeezed a booger out George Washington's nose.\n";
	yoMommas[104]="Yo momma so fat she stepped on a rainbow and made Skittles.\n";
	yoMommas[105]="Yo momma so fat she uses a mattress for a tampon.\n";
	yoMommas[106]="Yo momma so fat that when she sits on the beach, Greenpeace shows up and tries to tow her back into the ocean.....\n";
	yoMommas[107]="Yo momma so fat that she would have been in E.T., but when she rode the  bike across the moon, the bitch caused an eclipse.\n";
	yoMommas[108]="Yo momma so fat she hoola-hooped the super bowl.\n";
	yoMommas[109]="Yo momma so stupid that she puts lipstick on her head just to make-up her mind\n";
	yoMommas[110]="Yo momma so stupid she hears it's chilly outside so she gets a bowl\n";
	yoMommas[111]="Yo momma so stupid you have to dig for her IQ!\n";
	yoMommas[112]="Yo momma so stupid she got locked in a grocery store and starved!\n";
	yoMommas[113]="Yo momma so stupid it took her 2 hours to watch 60 Minutes!\n";
	yoMommas[114]="Yo momma so stupid that she tried to put M&M's in alphabetical order!\n";
	yoMommas[115]="Yo momma so supid she could trip over a cordless phone!\n";
	yoMommas[116]="Yo momma so stupid she sold her car for gasoline money!\n";
	yoMommas[117]="Yo momma so stupid she bought a solar-powered flashlight!\n";
	yoMommas[118]="Yo momma so stupid she thinks a quarterbacks a refund!\n";
	yoMommas[119]="Yo momma so stupid she took a cup to see Juice.\n";
	yoMommas[120]="Yo momma so stupid that she sold the car for gas money.\n";
	yoMommas[121]="Yo momma so stupid she asked you 'What is the number for 911'?\n";
	yoMommas[122]="Yo momma so stupid she took a ruler to bed to see how long she slept.\n";
	yoMommas[123]="Yo momma so stupid when she read on her job application to not write below the dotted line she put 'O.K.'\n";
	yoMommas[124]="Yo momma so stupid she got stabbed in a shoot out.\n";
	yoMommas[125]="Yo momma so stupid she stole free bread.\n";
	yoMommas[126]="Yo momma so stupid she took a spoon to the superbowl.\n";
	yoMommas[127]="Yo momma so stupid she called Dan Quayle for a spell check.\n";
	yoMommas[128]="Yo momma so stupid she stepped on a crack and broke her own back.\n";
	yoMommas[129]="Yo momma so stupid she makes Beavis and Butt-Head look like Nobel Prize winners.\n";
	yoMommas[130]="Yo momma so stupid she thought she needed a token to get on Soul Train.\n";
	yoMommas[131]="Yo momma so stupid when asked on an application, 'Sex?', she marked, 'M, F and sometimes Wednesday too.'\n";
	yoMommas[132]="Yo momma so stupid she took the Pepsi challenge and chose Jif.\n";
	yoMommas[133]="Yo momma so stupid when you stand next to her you hear the ocean!\n";
	yoMommas[134]="Yo momma so stupid she hears it's chilly outside so she gets a bowl\n";
	yoMommas[135]="Yo momma so stupid she got locked in a grocery store and starved!\n";
	yoMommas[136]="Yo momma so stupid she thinks Fleetwood Mac is a new hamburger at McDonalds!\n";
	yoMommas[137]="Yo momma so fat when her beeper goes off, people thought she was backing up\n";
	yoMommas[138]="Yo momma so fat her nickname is 'DAMN'\n";
	yoMommas[139]="Yo momma so fat she was baptised in the ocean.\n";
	yoMommas[140]="Yo momma so fat she has to iron her clothes in the driveway.\n";
	yoMommas[141]="Yo momma so fat they tie a rope around her shoulders and drag her through a tunnel when they want to clean it.\n";
	yoMommas[142]="Yo momma so fat when she got hit by a bus, she said, 'Who threw that rock?'\n";
	yoMommas[143]="Yo momma so fat when she stands in a left-turn lane it gives her the green arrow!\n";
	yoMommas[144]="Yo momma so fat that when whe was born, she gave the hospital stretch marks.\n";
	yoMommas[145]="Yo momma so fat the National Weather Agency has to assign names to her farts!!!\n";
	yoMommas[146]="Yo momma so fat we went to the drive-in and didn't have to pay because we dressed her as a Chevrolet.\n";
	yoMommas[147]="Yo momma so stupid it took her 2 hours to watch 60 minutes\n";
	yoMommas[148]="Yo momma so stupid when she saw the NC-17 (under 17 not admitted) sign, she went home and got 16 friends\n";
	yoMommas[149]="Yo momma so stupid when your dad said it was chilly outside, she ran outside with a spoon\n";
	yoMommas[150]="Yo momma so stupid she told everyone that she was 'illegitiment' because she couldn't read\n";
	yoMommas[151]="Yo momma so stupid she sits on the TV, and watches the couch!\n";
	yoMommas[152]="Yo momma so stupid that she thought Boyz II Men was a day care center.\n";
	yoMommas[153]="Yo momma so stupid she bought a videocamera to record cable tv shows at home.\n";
	yoMommas[154]="Yo momma so stupid when she went to take the 44 bus, she took the 22 twice instead.\n";
	yoMommas[155]="Yo momma so stupid she jumped out the window and went up.\n";
	yoMommas[156]="Yo momma so stupid she thought a quarterback was an income tax refund.\n";
	yoMommas[157]="Yo momma so stupid she took a umbrella to see Purple Rain.\n";
	yoMommas[158]="Yo momma so stupid that under 'Education' on her job apllication, she put 'Hooked on Phonics.'\n";
	yoMommas[159]="Yo momma so stupid she put out the cigarette butt that was heating your house.\n";
	yoMommas[160]="Yo momma so stupid she put lipstick on her forehead, talking about she was trying to makeup her mind.\n";
	yoMommas[161]="Yo momma so stupid she watches 'The Three Stooges' and takes notes.\n";
	yoMommas[162]="Yo momma so ugly when she joined an ugly contest, they said 'Sorry, no professionals.'\n";
	yoMommas[163]="Yo momma so ugly she looks out the window and got arrested for mooning.\n";
	yoMommas[164]="Yo momma so ugly just after she was born, her mother said 'What a treasure!' and her father said 'Yes, let's go bury it.'\n";
	yoMommas[165]="Yo momma so ugly they push her face into dough to make gorilla cookies.\n";
	yoMommas[166]="Yo momma so ugly they filmed 'Gorillas in the Mist' in her shower\n";
	yoMommas[167]="Yo momma so ugly they didn't give her a costume when she tried out for Star Wars.\n";
	yoMommas[168]="Yo momma so ugly instead of putting the bungee cord around her ankle, they put it around her neck\n";
	yoMommas[169]="Yo momma so ugly she gets 364 extra days to dress up for Halloween.\n";
	yoMommas[170]="Yo momma so ugly when she walks into a bank, they turn off the surveillence cameras\n";
	yoMommas[171]="Yo momma so ugly her mom had to be drunk to breast feed her\n";
	yoMommas[172]="Yo momma so ugly her mom had to tie a steak around her neck to get the dogs to play with her.\n";
	yoMommas[173]="Yo momma so ugly when she walks down the street in September, people say 'Damn, is it Halloween already?'\n";
	yoMommas[174]="Yo momma so ugly the government moved Halloween to her birthday.\n";
	yoMommas[175]="Yo momma so ugly that if ugly were bricks she'd have her own projects.\n";
	yoMommas[176]="Yo momma so ugly they pay her to put her clothes on in strip joints.\n";
	yoMommas[177]="Yo momma so ugly she made an onion cry.\n";
	yoMommas[178]="Yo momma so ugly they filmed 'Gorillas in the Mist' in her shower!\n";
	yoMommas[179]="Yo momma so ugly when they took her to the beautician it took 12 hours. . .for a quote!\n";
	yoMommas[180]="Yo momma so ugly they put her in dough and made monster cookies!\n";
	yoMommas[181]="Yo momma so ugly she tried to take a bath the water jumped out!\n";
	yoMommas[182]="Yo momma so ugly she looks out the window and gets arrested!\n";
	yoMommas[183]="Yo momma so ugly even Rice Krispies won't talk to her!\n";
	yoMommas[184]="Yo momma so ugly Ted Dansen wouldn't date her!\n";
	yoMommas[185]="Yo momma so ugly for Halloween she trick or treats on the phone!\n";
	yoMommas[186]="Yo momma so ugly she had to get her baby drunk to breastfeed it!\n";
	yoMommas[187]="Yo momma so ugly she turned Medusa to stone!\n";
	yoMommas[188]="Yo momma so ugly The NHL banned her for life\n";
	yoMommas[189]="Yo momma so ugly she gets 364 extra days to dress up for Halloween!\n";
	yoMommas[190]="Yo momma so ugly the government moved Halloween to her birthday!\n";
	yoMommas[191]="Yo momma so ugly if ugly were bricks she'd have her own projects!\n";
	yoMommas[192]="Yo momma so ugly they pay her to put her clothes on in strip joints\n";
	yoMommas[193]="Yo momma so ugly she made an onion cry!\n";
	yoMommas[194]="Yo momma so ugly people go as her for Halloween.\n";
	yoMommas[195]="Yo momma so ugly that when she sits in the sand on the beach, cats try to bury her.\n";
	yoMommas[196]="Yo momma so ugly she scares the roaches away.\n";
	yoMommas[197]="Yo momma so ugly we have to tie a steak around your neck so the dog will play with her!\n";
	yoMommas[198]="Yo momma so ugly I heard that your dad first met her at the pound.\n";
	yoMommas[199]="Yo momma so ugly that if ugly were bricks she'd have her own projects.\n";
	yoMommas[200]="Yo momma so ugly that your father takes her to work with him so that he doesn't have to kiss her goodbye.\n";
	yoMommas[201]="Yo momma so old I told her to act her own age, and the bitch died.\n";
	yoMommas[202]="Yo momma so old she has Jesus' beeper number!\n";
	yoMommas[203]="Yo momma so old her social security number is 1!\n";
	yoMommas[204]="Yo momma so old that when God said let the be light, she hit the switch'\n";
	yoMommas[205]="Yo momma so old that when she was in school there was no history class.\n";
	yoMommas[206]="Yo momma so old she owes Jesus 3 bucks!\n";
	yoMommas[207]="Yo momma so old she's in Jesus's yearbook!\n";
	yoMommas[208]="Yo momma so old she has a picture of Moses in her yearbook.\n";
	yoMommas[209]="Yo momma so old her birth certificate says expired on it.\n";
	yoMommas[210]="Yo momma so old she knew Burger King while he was still a prince.\n";
	yoMommas[211]="Yo momma so old she owes Jesus a nickel.\n";
	yoMommas[212]="Yo momma so old when God said 'Let their be light', she flipped the switch.\n";
	yoMommas[213]="Yo momma so old she was a waitress at the Last Supper.\n";
	yoMommas[214]="Yo momma so old she ran track with dinosaurs.\n";
	yoMommas[215]="Yo momma so old her birth certificate is in Roman numerals.\n";
	yoMommas[216]="Yo momma so old she has a picture of Moses in her yearbook.\n";
	yoMommas[217]="Yo momma so old she sat behind Jesus in the third grade.\n";
	yoMommas[218]="Yo momma so old and stupid she knew the Virgin Mary when she was 10 and said, 'Li'l Mary will never amount to anything'.\n";
	yoMommas[219]="Yo momma so poor when I saw her kicking a can down the street, I asked her what she was doing, she said 'Moving.'\n";
	yoMommas[220]="Yo momma so poor she can't afford to pay attention!\n";
	yoMommas[221]="Yo momma so poor when I ring the doorbell I hear the toilet flush!\n";
	yoMommas[222]="Yo momma so poor when she goes to KFC, she has to lick other people's fingers!!!\n";
	yoMommas[223]="Yo momma so poor when I ring the doorbell she says,'DING!'\n";
	yoMommas[224]="Yo momma so poor she went to McDonald's and put a milkshake on layaway.\n";
	yoMommas[225]="Yo momma so poor she went to McDonald's and put a milkshake on layaway.\n";
	yoMommas[226]="Yo momma so poor your family ate cereal with a fork to save milk.\n";
	yoMommas[227]="Yo momma so poor she goes to Kentucky Fried Chicken to lick other people's fingers.\n";
	yoMommas[228]="Yo momma so poor her face is on the front of a foodstamp.\n";
	yoMommas[229]="Yo momma so poor she was in K-Mart with a box of Hefty bags.  I said, 'What ya doin'?' She said, 'Buying luggage.'\n";
	yoMommas[230]="Yo momma so poor she drives a peanut.\n";
	yoMommas[231]="Yo momma so poor she waves around a popsicle stick and calls it air conditioning.\n";
	yoMommas[232]="Yo momma so dark she went to night school and was marked absent!\n";
	yoMommas[233]="Yo momma so dark she spits chocolate milk!\n";
	yoMommas[234]="Yo momma so dark she went to night school and was marked absent.\n";
	yoMommas[235]="Yo momma so dark that she can leave fingerprints on charcoal.\n";
	yoMommas[236]="Yo momma so dark she has to wear white gloves when she eats Tootsie Rolls to keep from eating her fingers.\n";
	yoMommas[237]="Yo momma so dirty she has to creep up on bathwater.\n";
	yoMommas[238]="Yo momma so short she poses for trophies!\n";
	yoMommas[239]="Yo momma so short you can see her feet on her drivers lisence!\n";
	yoMommas[240]="Yo momma so short she has to use a ladder to pick up a dime.\n";
	yoMommas[241]="Yo momma so short she can play handball on the curb.\n";
	yoMommas[242]="Yo momma so short she does backflips under the bed.\n";
	yoMommas[243]="Yo momma so short she models for trophys.\n";
	yoMommas[244]="Yo momma so nasty when she goes to a hair salon, she told the stylist to cut her hair and she opened up her shirt\n";
	yoMommas[245]="Yo momma so nasty She gotta put ice down her drawers to keep the crabs fresh!\n";
	yoMommas[246]="Yo momma so nasty she made speed stick slow down.\n";
	yoMommas[247]="Yo momma so nasty she brings crabs to the beach\n";
	yoMommas[248]="Yo momma so nasty she made right guard turn left.\n";
	yoMommas[249]="Yo momma so nasty the fishery be paying her to leave\n";
	yoMommas[250]="Yo momma so nasty she has to creep up on bathwater.\n";
	yoMommas[251]="Yo momma so nasty that her shit is glad to escape.\n";
	yoMommas[252]="Yo momma so nasty Ozzie Ozbourne refused to bite her head off\n";
	yoMommas[253]="Yo momma so nasty that pours salt water down her pants to keep her crabs fresh.\n";
	yoMommas[254]="Yo momma so nasty I called her for phone sex and she gave me an ear infection.\n";
	yoMommas[255]="Yo momma like potato chips-- Fri-to Lay\n";
	yoMommas[256]="Yo momma like a screen door, after a couple bangs she tends to loosen up! \n";
	yoMommas[257]="Yo momma like the pillbury doughboy - everyone gets a poke!\n";
	yoMommas[258]="Yo momma like a doorknob - everyone gets a turn!\n";
	yoMommas[259]="Yo momma like a T.V. set, even a three year old can turn her on!\n";
	yoMommas[260]="Yo momma like a doorknob, everyone gets a turn!\n";
	yoMommas[261]="Yo momma like a bus, fifty cents and she's ready to ride!\n";
	yoMommas[262]="Yo momma like a golf course, everyone GETS a hole in one!\n";
	yoMommas[263]="Yo momma like the railway system, she gets laid all over the country!\n";
	yoMommas[264]="Yo momma like a tomato source bottle, everyone gets a squeeze out of her!\n";
	yoMommas[265]="Yo momma like a shotgun: one cock and she blows!\n";
	yoMommas[266]="Yo momma like a hardware store: 4 cents a screw!\n";
	yoMommas[267]="Yo momma like Domino's pizza-- Something for nothing\n";
	yoMommas[268]="Yo momma like a refridgerator: everyone likes to put their meat in her!\n";
	yoMommas[269]="Yo momma like cake mix, 15 servings per package!\n";
	yoMommas[270]="Yo momma like a rifle... four cocks and she's loaded.\n";
	yoMommas[271]="Yo momma like a bowling ball.  She's picked up, fingered, and then thrown in the gutter.\n";
	yoMommas[272]="Yo momma like a bus:  Guys climb on and off her all day long.\n";
	yoMommas[273]="Yo momma like a Toyota: 'Oh what a feelin'!'\n";
	yoMommas[274]="Yo momma like Orange Crush: 'Good Vibrations!'\n";
	yoMommas[275]="Yo momma like a bubble-gum machine... five cents a blow.\n";
	yoMommas[276]="Yo momma like chinese food:  sweet, sour and cheap!\n";
	yoMommas[277]="Yo momma like a vaccuum cleaner.....a real good suck.\n";
	yoMommas[278]="Yo momma so hairy you almost died of rugburn at birth!\n";
	yoMommas[279]="Yo momma so hairy she's got afros on her nipples!\n";
	yoMommas[280]="Yo momma so hairy she look like she got Buchwheat in a headlock.\n";
	yoMommas[281]="Yo momma so hairy Bigfoot is taking her picture!\n";
	yoMommas[282]="Yo momma so hairy she wears a Nike tag on her weave so now everybody calls her Hair Jordan.\n";
	yoMommas[283]="Yo momma so slutty she could suck-start a Harley!\n";
	yoMommas[284]="Yo momma so slutty she could suck the chrome off a trailer hitch ball!\n";
	yoMommas[285]="Yo momma so slutty when she got a new mini skirt, everyone commented on her nice belt!\n";
	yoMommas[286]="Yo momma so slutty she was on the cover of wheaties, with her legs open, and it said 'breakfast of the champs' \n";
	yoMommas[287]="Yo momma so slutty that I could've been your daddy, but the guy in line behind me had the correct change.\n";
	yoMommas[288]="Yo momma so slutty she had her own 'Hands across her ass' charity drive\n";
	yoMommas[289]="Yo momma so slutty that when she heard Santa Claus say HO HO HO she thought she was getting it three times.\n";
	yoMommas[290]="Yo momma so slutty I fucked her and I's a chick!\n";
	yoMommas[291]="Yo momma so slutty she blind and seeing another man.\n";
	yoMommas[292]="Yo momma nose so big you can go bowling with her boogers!\n";
	yoMommas[293]="Yo momma nose so big she makes Pinochio look like a cat!\n";
	yoMommas[294]="Yo momma so greasy she used bacon as a band-aid!\n";
	yoMommas[295]="Yo momma so greasy she sweats Crisco!\n";
	yoMommas[296]="Yo momma so greasy Texaco buys Oil from her\n";
	yoMommas[297]="Yo momma teeth are so yellow traffic slows down when she smiles!\n";
	yoMommas[298]="Yo momma teeth are so yellow she spits butter!\n";
	yoMommas[299]="Yo momma so lazy she thinks a two-income family is where yo daddy has two jobs.\n";
	yoMommas[300]="Yo momma so skinny she hula hoops with a cheerio\n";
	yoMommas[301]="Yo momma so skinny she has to wear a belt with spandex.\n";
	yoMommas[302]="Yo momma so skinny she turned sideways and dissapeared.\n";
	yoMommas[303]="Yo momma so bald even a wig wouldn't help!\n";
	yoMommas[304]="Yo momma so bald you can see whats on her mind\n";
	yoMommas[305]="Yo momma so bald that she took a shower and got brain-washed.\n";
	yoMommas[306]="Yo momma so tall she tripped over a rock and hit her head on the moon.\n";
	yoMommas[307]="Yo momma so tall she did a back-flip and kicked Jesus in the mouth.\n";
	yoMommas[308]="Yo momma so tall she tripped in Michigan and hit her head in Florida.\n";
	yoMommas[309]="Yo momma so flat she's jealous of the wall!\n";
	yoMommas[310]="Yo momma's glasses are so thick that when she looks on a map she can see people waving.\n";
	yoMommas[311]="Yo momma's glasses are so thick she can see into the future.\n";
	yoMommas[312]="Yo momma has an afro with a chin strap.\n";
	yoMommas[313]="Yo momma has one leg and a bicycle.\n";
	yoMommas[314]="Yo momma has 4 eyes and 2 pair of sunglasses.\n";
	yoMommas[315]="Yo momma has so much hair on her upper lip, she braids it.\n";
	yoMommas[316]="Yo momma has one hand and a Clapper.\n";
	yoMommas[317]="Yo momma has a wooden afro with an 'X' carved in the back.\n";
	yoMommas[318]="Yo momma has green hair and thinks she's a tree.\n";
	yoMommas[319]="Yo momma has one ear and has to take off her hat to hear what you're saying.\n";
	yoMommas[320]="Yo momma has a 'fro with warning lights.\n";
	yoMommas[321]="Yo momma has 10 fingers--all on the same hand.\n";
	yoMommas[322]="Yo momma has a glass eye with a fish in it.\n";
	yoMommas[323]="Yo momma has a short leg and walks in circles.\n";
	yoMommas[324]="Yo momma has a short arm and can't applaude.\n";
	yoMommas[325]="Yo momma got so many freckles she looks like a hamburger!\n";
	yoMommas[326]="Yo momma got two wooden legs and one is one backward.\n";
	yoMommas[327]="Yo momma got three fingers and a banjo.\n";
	yoMommas[328]="Yo momma got a wooden leg with a kickstand on it.\n";
	yoMommas[329]="Yo momma got a bald head with a part and sideburns.\n";
	yoMommas[330]="Yo momma got a' afro, wit' a chin strap!!!!\n";
	yoMommas[331]="Yo momma got a wooden leg with branches.\n";
	yoMommas[332]="Yo momma house so small you have to go outside to change your mind.\n";
	yoMommas[333]="Yo momma house so dirty roaches ride around on dune buggies!\n";
	yoMommas[334]="Yo momma house so dirty she has to wipe her feet before she goes outside.\n";
	yoMommas[335]="Yo momma hair so short when she braided it they looked like stiches.\n";
	yoMommas[336]="Yo momma hair so short she curls it with rice.\n";
	yoMommas[337]="Yo momma head so big she has to step into her shirts.\n";
	yoMommas[338]="Yo momma head so big it shows up on radar.\n";
	yoMommas[339]="Yo momma head so small she use a tea-bag as a pillow.\n";
	yoMommas[340]="Yo momma head so small that she got her ear pierced and died.\n";
	yoMommas[341]="Yo momma wears knee-pads and yells 'Curb Service!'\n";
	yoMommas[342]="Yo momma feet are so big her shoes have to have license plates!\n";
	yoMommas[343]="Yo momma aint so bad...she would give you the hair off of her back!\n";
	yoMommas[344]="Yo momma lips so big, Chap Stick had to invent a spray.\n";
	yoMommas[345]="Yo mamma is so fat, she has to use a satellite dish for a diaphragm.\n";
	yoMommas[346]="Yo momma hips are so big, people set their drinks on them.\n";
	yoMommas[347]="Yo momma hair so nappy she has to take Tylenol just to comb it.\n";
	yoMommas[348]="Yo momma so clumsy she got tangled up in a cordless phone.\n";
	yoMommas[349]="Yo momma so wrinkled, she has to screw her hat on.\n";
	yoMommas[350]="Yo momma twice the man you are.\n";
	yoMommas[351]="Yo momma cross-eyed and watches TV in stereo.\n";
	yoMommas[352]="Yo momma is missing a finger and can't count past 9.\n";
	yoMommas[353]="Yo momma arms are so short, she has to tilt her head to scratch her ear.\n";
	yoMommas[354]="Yo momma middle name is Rambo.\n";
	yoMommas[355]="Yo momma in a wheelchair and says, 'You ain't gonna puch me 'round no more.'\n";
	yoMommas[356]="Yo momma rouchy, the McDonalds she works in doesn't even serve Happy Meals.\n";
	yoMommas[357]="Yo momma so stupid was born on Independence Day and can't remember her birthday.\n";
	yoMommas[358]="Yo momma mouth so big, she speaks in surround sound.\n";
	yoMommas[359]="Yo momma gums are so black she spits Yoo-hoo.\n";
	yoMommas[360]="Yo momma breath smell so bad when she yawns her teeth duck.\n";
	yoMommas[361]="Yo momma teeth are so rotten, when she smiles they look like dice.\n";
	yoMommas[362]="Yo mamma so fat, she wore a X t-shirt, and a helicopter landed on her\n";
	yoMommas[363]="Yo mamma so skinny, she hang glides with a Dorito\n";
	yoMommas[364]="Yo mamma so stupid, she thinks hamburger helper comes with another person\n";
	yoMommas[365]="Yo mamma so stupid, she thinks menopause is a button\n";
	yoMommas[366]="Yo mamma is so fat - she's got more chins than chinatown!\n";
	yoMommas[367]="Yo mamma is so fat that you gotta grease the tub to get her in the bath!\n";
	yoMommas[368]="Yo mamma is so fat, she has to use a satellite dish for a diaphragm.\n";
	yoMommas[369]="Yo mamma is so fat that cars run out of gas just trying to pass her ass!\n";
	yoMommas[370]="Yo mamma is so fat she's gotta sleep in a barn with the rest of the cows.\n";
	yoMommas[371]="Yo mamma is like a hoover, turn her on and she sucks!\n";
	yoMommas[372]="Yo mamma is like an elevator - if you push the right button she'll go down on you!\n";
	yoMommas[373]="Yo mamma is like a shotgun... one cock and she's ready to blow.\n";
	yoMommas[374]="Yo mamma is like a racecar - she can burn four rubbers in one night!\n";
	yoMommas[375]="Yo momma is like a subway... everyone rides her!\n";
	yoMommas[376]="Yo mamma has a pussy on her hip, so she can make money on the side!\n";
	yoMommas[377]="Yo mamma is so fat that she irons her clothes in the driveway!\n";
	yoMommas[378]="Yo mamma is like a TV set, anyone can turn her on.\n";
	yoMommas[379]="Yo mamma is like a gas station - eventually everyone gets a pump.\n";
	yoMommas[380]="Yo mamma is so fat that she had to get baptized at sea world\n";
	yoMommas[381]="Yo mamma is so fat that they use her underwear for bungee jumping!\n";
	yoMommas[382]="Yo mamma is so fat she ordered a double room for a singles weekend!\n";




	return 0;
}
 
/** @brief The LKM cleanup function
 *  Similar to the initialization function, it is static. The __exit macro notifies that if this
 *  code is used for a built-in driver (not a LKM) that this function is not required.
 */
static void __exit yoMomma_exit(void){
	device_destroy(yoMommaClass, MKDEV(majorNumber, 0));     // remove the device
	class_unregister(yoMommaClass);                          // unregister the device class
	class_destroy(yoMommaClass);                             // remove the device class
	unregister_chrdev(majorNumber, DEVICE_NAME);             // unregister the major number
	printk(KERN_INFO "yoMomma: Goodbye from the LKM!\n");
}
 
/** @brief The device open function that is called each time the device is opened
 *  This will only increment the numberOpens counter in this case.
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_open(struct inode *inodep, struct file *filep){
	numberOpens++;
	printk(KERN_INFO "yoMomma: Device has been opened %d time(s)\n", numberOpens);
	return 0;
}
 
/** @brief This function is called whenever device is being read from user space i.e. data is
 *  being sent from the device to the user. In this case is uses the copy_to_user() function to
 *  send the buffer string to the user and captures any errors.
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param len The length of the b
 *  @param offset The offset if required
 */
 static int index = 0;
 static int flag = 0;
  static int wrote = 0;
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
	
		if (wrote == 1) {
			copy_to_user(buffer, "What did you say to me? Screw you..\n", strlen("What did you say to me? Screw you..\n"));
			wrote = 0;
			flag = 1;
			return strlen("What did you say to me? Screw you..\n");
		}
  	if (index == 744) {
  		index = 0;
  		return 0;
  	}
  	if (flag == 1) {
  		flag = 0;
  		return 0;
  	}
	copy_to_user(buffer, yoMommas[index], strlen(yoMommas[index]));
	flag = 1;
	return strlen(yoMommas[index++]);
}
 
/** @brief This function is called whenever the device is being written to from user space i.e.
 *  data is sent to the device from the user. The data is copied to the message[] array in this
 *  LKM using the sprintf() function along with the length of the string.
 *  @param filep A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param len The length of the array of data that is being passed in the const char buffer
 *  @param offset The offset if required
 */

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
	wrote = 1; 
	return len;
}
 
/** @brief The device release function that is called whenever the device is closed/released by
 *  the userspace program
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_release(struct inode *inodep, struct file *filep){
	printk(KERN_INFO "yoMomma: Device successfully closed\n");
	return 0;
}
 
 
MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("Dawood Alnajjar");    ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("Yo Mama driver");  ///< The description -- see modinfo
MODULE_VERSION("0.1");            ///< A version number to inform users

/** @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(yoMomma_init);
module_exit(yoMomma_exit);
