#ifndef GENERATOR_H_
#define GENERATOR_H_
#include <cassert>
#include "DatabaseModel.h"
#include "Random.h"
#include <memory>

// that class can generate random database items and populate database with it

using namespace project;


class Generator
{
	const unsigned STUDENTS_NUMBER;
	const unsigned CLASSES_NUMBER;
	const unsigned PROFESSORS_NUMBER;
	const unsigned MARKS_NUMBER;
	static const std::vector < std::string> surnames;
	static const std::vector<std::string> names;
	static const std::vector<std::string> cities;
	static const std::vector<std::string> streets;
public:
	std::vector<StudentInfo> m_students;
	std::vector<ClassInfo> m_classes;
	std::vector<ProfessorInfo> m_professors;
	std::vector<MarkInfo> m_marks;

	Generator(unsigned students = 1000, unsigned classes = 50, unsigned professors = 200, unsigned marks = 5000)
		: STUDENTS_NUMBER(students), CLASSES_NUMBER(classes), PROFESSORS_NUMBER(professors), MARKS_NUMBER(marks)
	{
		m_students = generate_random_studends();
		m_classes = generate_random_classes();
		m_professors = generate_random_professors();
		m_marks = generate_random_marks();
	}
	void populate(DatabaseModel& db)
	{
		_populate(db, m_classes);
		_populate(db, m_professors);
		_populate(db, m_students);
		_populate(db, m_marks);	
	}
private:
	template <class InfoVec>
	void _populate(DatabaseModel& db, InfoVec& infovec)
	{
		for (auto& i : infovec)
		{
			db.add(i);
		}
	}
	std::vector<StudentInfo> generate_random_studends()
	{
		unsigned size = STUDENTS_NUMBER;
		std::vector<StudentInfo> vec;
		vec.reserve(size);
		Random rand{};
		while (size-- != 0)
		{
			vec.emplace(vec.end(), get_name(rand), get_surname(rand), PESEL_gen(rand), get_city(rand), get_street(rand) + " " + rand.number_token(2), rand.random_int(1, CLASSES_NUMBER));
		}
		return vec;
	}
	std::vector<ClassInfo> generate_random_classes()
	{
		unsigned size = CLASSES_NUMBER;
		std::vector<ClassInfo> vec;
		vec.reserve(size);
		Random rand{};
		while (size-- != 0)
		{
			vec.emplace(vec.end(), rand.token(4), rand.random_int(1, 8));
		}
		return vec;
	}

	std::vector<ProfessorInfo> generate_random_professors()
	{
		unsigned size = PROFESSORS_NUMBER;
		std::vector<ProfessorInfo> vec;
		vec.reserve(size);
		Random rand{};
		while (size-- != 0)
		{
			vec.emplace(vec.end(), get_name(rand), get_surname(rand), rand.token(3), std::vector<std::string>{rand.token(2), rand.token(2)}, PESEL_gen(rand));
		}
		return vec;
	}

	std::vector<MarkInfo> generate_random_marks()
	{
		unsigned size = MARKS_NUMBER;
		std::vector<MarkInfo> vec;
		vec.reserve(size);
		Random rand{};
		while (size-- != 0)
		{
			vec.emplace(vec.end(), rand.random_int(1, STUDENTS_NUMBER), static_cast<MARK>(rand.random_int(4, 10)), get_date(rand), static_cast<SUBJECT>(rand.random_int(0, 2)), rand.random_int(1, PROFESSORS_NUMBER));
		}
		return vec;
	}


private:
	std::string PESEL_gen(Random& rand)
	{
		static unsigned pesel = 9000;
		std::string str{ std::to_string(pesel++) };
		str += rand.number_token(11);
		return str.substr(0, 11);
	}

	std::string get_name(Random& rand)
	{
		return names[rand.random_int(0, names.size() - 1)];
	}

	std::string get_surname(Random& rand)
	{
		return surnames[rand.random_int(0, surnames.size() - 1)];
	}

	std::string get_city(Random& rand)
	{
		return cities[rand.random_int(0, cities.size() - 1)];
	}

	std::string get_street(Random& rand)
	{
		return streets[rand.random_int(0, streets.size() - 1)];
	}

	std::string get_date(Random& rand)
	{
		auto day = std::to_string(rand.random_int(1, 27));
		auto month = std::to_string(rand.random_int(1, 12));
		auto year = "2018";
		return day + '-' + month + '-' + year;
	}

	


};

const std::vector<std::string> Generator::surnames{ "SMITH", "JOHNSON", "WILLIAMS", "BROWN", "JONES", "GARCIA", "RODRIGUEZ", "MILLER", "MARTINEZ", "DAVIS", "HERNANDEZ", "LOPEZ", "GONZALEZ", "WILSON", "ANDERSON", "THOMAS", "TAYLOR", "LEE", "MOORE", "JACKSON", "PEREZ", "MARTIN", "THOMPSON", "WHITE", "SANCHEZ", "HARRIS", "RAMIREZ", "CLARK", "LEWIS", "ROBINSON", "WALKER", "YOUNG", "HALL", "ALLEN", "TORRES", "NGUYEN", "WRIGHT", "FLORES", "KING", "SCOTT", "RIVERA", "GREEN", "HILL", "ADAMS", "BAKER", "NELSON", "MITCHELL", "CAMPBELL", "GOMEZ", "CARTER", "ROBERTS", "DIAZ", "PHILLIPS", "EVANS", "TURNER", "REYES", "CRUZ", "PARKER", "EDWARDS", "COLLINS", "STEWART", "MORRIS", "MORALES", "ORTIZ", "GUTIERREZ", "MURPHY", "ROGERS", "COOK", "KIM", "MORGAN", "COOPER", "RAMOS", "PETERSON", "GONZALES", "BELL", "REED", "BAILEY", "CHAVEZ", "KELLY", "HOWARD", "RICHARDSON", "WARD", "COX", "RUIZ", "BROOKS", "WATSON", "WOOD", "JAMES", "MENDOZA", "GRAY", "BENNETT", "ALVAREZ", "CASTILLO", "PRICE", "HUGHES", "VASQUEZ", "SANDERS", "JIMENEZ", "LONG", "FOSTER" };
const std::vector<std::string> Generator::names{ "Michael", "James", "John", "Robert", "David", "William", "Mary", "Christopher", "Joseph", "Richard", "Daniel", "Thomas", "Matthew", "Jennifer", "Charles", "Anthony", "Patricia", "Linda", "Mark", "Elizabeth", "Joshua", "Steven", "Andrew", "Kevin", "Brian", "Barbara", "Jessica", "Jason", "Susan", "Timothy", "Paul", "Kenneth", "Lisa", "Ryan", "Sarah", "Karen", "Jeffrey", "Donald", "Ashley", "Eric", "Jacob", "Nicholas", "Jonathan", "Ronald", "Michelle", "Kimberly", "Nancy", "Justin", "Sandra", "Amanda", "Brandon", "Stephanie", "Emily", "Melissa", "Gary", "Edward", "Stephen", "Scott", "George", "Donna", "Jose", "Rebecca", "Deborah", "Laura", "Cynthia", "Carol", "Amy", "Margaret", "Gregory", "Sharon", "Larry", "Angela", "Maria", "Alexander", "Benjamin", "Nicole", "Kathleen", "Patrick", "Samantha", "Tyler", "Samuel", "Betty", "Brenda", "Pamela", "Aaron", "Kelly", "Heather", "Rachel", "Adam", "Christine", "Zachary", "Debra", "Katherine", "Dennis", "Nathan", "Christina", "Julie", "Jordan", "Kyle", "Anna", };
const std::vector<std::string> Generator::cities{ "New York","Los Angeles","Chicago","Houston","Philadelphia","Phoenix","San Antonio","San Diego","Dallas","San Jose","Austin","Indianapolis","Jacksonville","San Francisco","Columbus","Charlotte","Fort Worth","Detroit","El Paso","Memphis","Seattle","Denver","Washington","Boston","Nashville-Davidson","Baltimore","Oklahoma City","Louisville/Jefferson County","Portland","Las Vegas","Milwaukee","Albuquerque","Tucson","Fresno","Sacramento","Long Beach","Kansas City","Mesa","Virginia Beach","Atlanta","Colorado Springs","Omaha","Raleigh","Miami","Oakland","Minneapolis","Tulsa","Cleveland","Wichita","Arlington","New Orleans","Bakersfield","Tampa","Honolulu","Aurora","Anaheim","Santa Ana","St. Louis","Riverside","Corpus Christi","Lexington-Fayette","Pittsburgh","Anchorage","Stockton","Cincinnati","St. Paul","Toledo","Greensboro","Newark","Plano","Henderson","Lincoln","Buffalo","Jersey City","Chula Vista","Fort Wayne","Orlando", "Petersburg","Chandler","Laredo","Norfolk","Durham","Madison","Lubbock","Irvine","Winston - Salem","Glendale","Garland","Hialeah","Reno","Chesapeake","Gilbert","Baton Rouge","Irving","Scottsdale","North Las Vegas","Fremont","Boise City","Richmond","San Bernardino" };
const std::vector<std::string> Generator::streets{ "Adams","Air Cargo","Alaska","Alaska Service","Alder","Alderdale","Alderwood Mall","Alexander","Algonquin","Aloha","Alvord","Ames","Andover","Appleton","Armour","Armstrong","Atlas","Avalon","Avondale","Baker","Balder","Baldwin","Barker","Bartlett","Battery","Bay","Bayview","Beach","Bear Creek","Beardslee","Beck","Beech","Bel Red","Belfair","Bell","Bella Coola","Bellefield Park","Bellevue Redmond","Bellflower","Ben Howard","Benson","Benton","Berry","Bing","Birch","Bjune","Blackford","Blaine","Blanchard","Blanche","Blomskog","Blue Ridge","Boeing Access","Bogny","Bonney","Bonnie Brook","Boren","Bostian","Boston","Boundary","Braemar","Brew","Brien","Brier","Broad","Broadway","Brook","Brook Bay","Brookmere","Broomgerrie","Brown","Brygger","Butterworth","Cadman Quarry","Cafe On The","Cafe on the","Canal","Canyon","Carmichael","Carol","Carter","Cary","Cascade","Caspers","Castle","Cedar","Cedar Crest","Cedar Falls","Cedar River Park","Cedar River Pipeline","Cedar Valley","Cedars East","Chelan","Cherry","Cherry Valley","Chism Park","Christensen","Circle","Clallam","Clark","Clay","Clay Pit","Cleveland","Clover","Club House","Clyde","Coal Creek","Cocker Creek","Coho","Cole","Colonial","Colorado","Columbia","Comstock","Cooper","Cowlitz","Crawford","Cremona","Crescent Lake","Crockett","Crystal Lake","Daley","Damson","Dawn","Dawson","Deerford","Dellwood","Des Moines Memorial","Detwiller","Dexter","Dike","Ditch","Dock","Dogwood","Donlan","Douglas","Downes","Dravus","Driftwood","Duchess","Dugway","Durbin","E Martin","Eagle","Eagle Harbor","Eason","East","East Alder","East Allison","East Aloha","East Bagwell","East Bird","East Blaine","East Boston","East Calhoun","East Cherry","East Columbia","East Commercial","East Crescent","East Crockett","East Echo Lake","East Edgar","East Eugene","East Fir","East Foster Island","East Galer","East Garfield","East George","East Glen","East Gowe","East Greystone","East Hamlin","East Harrison","East Helen","East High","East Highland","East Howe","East Howell","East Huron","East Interlaken","East Interurban","East James","East Jefferson","East John","East Jonathan","East Lake Kayak","East Lake Washington","East Lee","East Lost Lake","East Louisa","East Lousia","East Lynn","East Madison","East Main","East Marion","East Mc Gilvra","East Mc Graw","East Meeker","East Mercer","East Mercer Highland","East Miller","East Morrison","East Newton","East North","East Novak","East Olive","East Pike","East Pine","East Prospect","East Reitze","East Republican","East Riverside","East Roanoke","East Roy","East Rutherford","East Seneca","East Shore","East Shorewood","East Smith","East Spring","East Spruce","East Superior","East Tacoma","East Temperance","East Terrace","East Thomas","East Titus","East Union","East Valley","East Ward","Eastlake","Eden","Edmonds","Edwards","El Dorado Beach Club","Elberta","Eldorado","Elephant","Elliott","Elm","Emerald Hills","Enatai","Entwistle","Erben","Erie","Etruria","Euclid","Evans Black","Evergreen","Evergreen Point","Faben","Fairview","Fairweather","Fales","Fern Hollow","Fernbrook","Ferncroft","Fernridge","Filbert","Fir","Firdale","Firwood","Florentia","Forbes Creek","Forest Dell","Forsyth","Fort","Fortuna","Freeman","Friar Tuck","Frontage","Fullerton","Fulton","Galer","Garfield","Gateway","George Washington","Gilman","Giltner","Glen","Glenhome","Glenridge","Grand","Grand Ridge","Grandview","Grannis","Grant","Gravel","Gravenstein","Green Valley","Greenbrier","Greening","Grimes","Groat Point","Hall","Halladay","Halverson","Hamilton","Hamlin Park","Hampton","Hanna Park","Harrington","Harrison","Harvard","Harvest","Hayes","Hazel","Hedlund","Hellen","Hemlock","High","Highland","Highland Park","Highlands","Highmoor","Hildebrand","Hillcrest","Hillside","Hilltop","Hindley","Hoeder","Holly","Holly Hill","Homeland","Homeview","Horton","Howe","Howell","Hubbard","Huckleberry","Humber","Hunts Point","Hurst","Illinios","Industry","International","Interurban","Island","Island Heights","Issaquah Hobart","James","Jason","Jefferson","Jewell","John","John Bailey","Jonathan","Kansas","Kayak Lake","Kelsy Creek","Kenosia","Kensington","Kentish","Kerriston","Kerry","King","Kings Lake","Kirkland","Kitsap","Klickitat","Klockstad","Kotschevar","Kulshan","Lac Lehman","Lake","Lake Bellevue","Lake Dell","Lake Fontal","Lake Hills","Lake Hills Connector","Lake Shore","Lake Washington","Lakehurst","Lakeshore Plaza","Lakeside","Lakeview","Landing","Lansdowne","Laurel","Lawton","Lawtonwood","Lee","Lenora","Lewis","Lincoln","Linda","Lindley","Little Bear Creek","Lockwood","Logan","Logging","Lost Lake","Lotus","Lucile","Lynn","Macarthur","Madison","Madonna","Madrona","Magnolia","Main","Main Tiger Mountain","Makah","Mallard","Maple","Maplewood","Marine View","Marion","Market","Marymoor Park","Mason","Mattson","Maule","Mc Graw","Mc Millan","McGraw","McKinley","Meadow","Meadowdale","Meadowdale Beach","Melody","Melrose","Mercer","Mercer Terrace","Mercerwood","Meridian","Merrimount","Midland","Milwaukee","Minkler","Minor","Monroe Duvall","Monte Villa","Moss","Mount Baker","Mount Forest","Mountain","Myrtle","NE Allen","NE Greens Crossing","Nellis","Nels Berglund","Newcastle Coal Creek","Newcastle Golf Club","Newell","News","Newton","Nickerson","Nike Manor","Nootka","North","North Brooks","North Canal","North Creek","North Danvers","North Deer","North Dogwood","North Echo Lake","North Greenwood","North High Rock","North Kennebeck","North Lenora","North Marion","North Market","North Pacific","North Richmond Beach","North Riverside","Northbrook","Northeast Albertson","Northeast Alder","Northeast Alder Crest","Northeast Alderwood","Northeast Ambleside","Northeast Ames Lake","Northeast Anderson","Northeast Apple Cove","Northeast Arness","Northeast Arrowhead","Northeast Avalon","Northeast Baker Hill","Northeast Beach Crest","Northeast Beachwood","Northeast Beadonhall","Northeast Beck","Northeast Belle Hill","Northeast Berry","Northeast Big Rock","Northeast Bill Point","Northeast Birch","Northeast Bird","Northeast Blackster","Northeast Blakeley","Northeast Boat","Northeast Brackenwood","Northeast Brooklyn","Northeast Brownell","Northeast Burns","Northeast Byron","Northeast California","Northeast Campus","Northeast Carpenter","Northeast Carriage","Northeast Casey","Northeast Cherry","Northeast Clare","Northeast Comegys","Northeast Coral","Northeast County Park","Northeast Coyote","Northeast Crescent","Northeast D","Northeast Daphne","Northeast Darby","Northeast Darden","Northeast Day","Northeast Delaney","Northeast Dingley","Northeast Discovery","Northeast Dogwood","Northeast Dorothy","Northeast Douglas","Northeast Eaton","Northeast Endicott","Northeast Erin","Northeast Evergreen","Northeast Ewing","Northeast Federal","Northeast Felicity","Northeast Fenton","Northeast Fir","Northeast Georgia","Northeast Gilman","Northeast Gisle","Northeast Glavin","Northeast Goodfellow","Northeast Gordon","Northeast Grizdale","Northeast Halls Hill","Northeast Hansen","Northeast Harris","Northeast Harrison","Northeast Hawthorne","Northeast Hidden Cove","Northeast High","Northeast High School","Northeast Hillside","Northeast Hilltop","Northeast Hollyhills","Northeast Huckleberry","Northeast Husky","Northeast Iris","Northeast Iverson","Northeast Jade","Northeast Jewell","Northeast John","Northeast Johnson","Northeast Jonquil","Northeast Joshua Tree","Northeast Juanita","Northeast Julep","Northeast Juniper","Northeast Karmenn","Northeast Katsura","Northeast Kelsey","Northeast Kenilworth","Northeast Kennedy","Northeast Kenwood","Northeast Keswick","Northeast Killian","Northeast Kitsap","Northeast Kiwi","Northeast Klabo","Northeast Knight","Northeast Koura","Northeast Lacey","Northeast Lafayette","Northeast Lake Joy","Northeast Larchmount","Northeast Laurel Wood","Northeast Laurelcrest","Northeast Leprechaun","Northeast Lofgren","Northeast Logan","Northeast Loughrey","Northeast Lovgren","Northeast Mabrey","Northeast Magnolia","Northeast Maine","Northeast Manor","Northeast Maple","Northeast Marine View","Northeast Marion","Northeast Marketplace","Northeast Mary Lou","Northeast McRedmond","Northeast Meadowmeer","Northeast Meigs","Northeast Meyers","Northeast Michelle","Northeast Midway","Northeast Miller","Northeast Monroe","Northeast Monsaas","Northeast Morgan","Northeast Morning","Northeast Moses","Northeast Mulberry","Northeast Munson","Northeast Murden Cove","Northeast NOAA","Northeast Noble","Northeast Northstar","Northeast Norton","Northeast Ocean","Northeast Oddfellows","Northeast Ohio","Northeast Olive","Northeast Oregon","Northeast Pacific","Northeast Park","Northeast Paulanna","Northeast Penrith","Northeast Phillip","Northeast Pine","Northeast Point View","Northeast Points","Northeast Preston","Northeast Puget","Northeast Puget Bluff","Northeast Quail Creek","Northeast Raccoon","Northeast Radford","Northeast Rasperry","Northeast Ravenna","Northeast Redmond","Northeast Reny","Northeast Richardson","Northeast Ring","Northeast Roberts","Northeast Roney","Northeast Rotsten","Northeast Rupard","Northeast Sasquatch","Northeast Seaborn","Northeast Seaview","Northeast Shore","Northeast South Beach","Northeast South Villa","Northeast Sprayfalls","Northeast Springwood","Northeast Spruce","Northeast Sunrose","Northeast Sunset","Northeast Tani Creek","Northeast Theresa","Northeast Tolt Hill","Northeast Torvanger","Northeast Tulin","Northeast Union Hill","Northeast Valley","Northeast Victorian","Northeast Viewcrest","Northeast en","Woodinville","Woodlake","Woodland","Woodway Park","Yakima","Yale","York" };



#endif // !GENERATOR_H_