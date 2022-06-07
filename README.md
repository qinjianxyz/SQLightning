# SQLightning-Engineering-Notebook

### SQLighning is a SQL command line terminal written in C++.


### Getting Started
step 1: make<br>
step 2: ./sqlightning<br>

### Example Commands:<br>
run <filename>;<br>
help;<br>
version;<br>
quit;<br>
CREATE DATABASE test1;<br>
USE test1;<br>
CREATE TABLE Users ( id int NOT NULL auto_increment primary key, first_name varchar(50) NOT NULL, last_name varchar(50), age int, zipcode int);<br>
create table Books ( id int NOT NULL auto_increment primary key, title varchar(25) NOT NULL, subtitle varchar(25), user_id int);<br>
DESCRIBE Users;<br>
DESCRIBE Books;<br>
INSERT INTO Users (first_name, last_name, age, zipcode) VALUES ("Terry",     "Pratchett", 70,  92124), ("Ian",       "Tregellis", 48,  92123), ("Jody",      "Taylor",    50,  92120), ("Stephen",   "King",      74,  92125), ("Ted",       "Chiang",    56,  92120), ("Anthony",   "Doerr",     52,  92122), ("J.R.R.",    "Tolkien",   130, 92126), ("Aurthur C.","Clarke",    105, 92127), ("Seldon",    "Edwards",   81,  92128), ("Neal",      "Stephenson",62,  92121);<br>
INSERT INTO Books (title, user_id) VALUES ("The Green Mile",4), ("The Stand",4), ("Misery",4), ("11/22/63",4), ("The Institute",4), ("Sorcerer",1), ("Wintersmith",1), ("Mort",1), ("Thud",1), ("Time Police",3), ("The Mechanical",2), ("The Liberation",2), ("The Rising",2), ("Exhalation",5);<br>
SELECT * from Users;<br>
SELECT * from Users  limit 3 where zipcode>92122 order by zipcode;<br>
select * from Users order by last_name;<br>
select * from Books order by title;<br>
SELECT * from Users where age>20 AND zipcode=92100 ORDER BY last_name;<br>
select first_name, last_name, age from Users order by last_name where age>60;<br>
UPDATE Users SET "zipcode" = 92127 WHERE zipcode>92100;<br>
DELETE FROM Users WHERE zipcode>92000;<br>
ALTER TABLE Books ADD  pub_year int;<br>
  ALTER TABLE Books DROP subtitle;<br>
DROP TABLE Users;<br>
SHOW TABLES;<br>
SHOW DATABASES;<br>
DROP DATABASE test1;<br>
DUMP DATABASE test1;<br>
SHOW INDEXES;<br>
SHOW INDEX id FROM Users;<br>
select last_name, title from Authors left join Books on Authors.id=Books.author_id;<br>
select last_name, title from Authors right join Books on Authors.id=Books.author_id;<br>

### Engineering Notebook
  
### Week 1 Update
The most significant thing we learned is how tokenizer works and why it is important to convert raw texts in to symbols that the system can recognize. I am pondering the exact definition of abstraction whose definition loosely relates to hiding details and indirection. I learned key components of a database such as entities, languages, tables, interpreter, indexes, caching, etc. We implemented a few simple commands such as quit version and help. It gives me an overview of how the tokenizer system works. In terms of future plan for the next week, I need to familiarize myself with the codebase and gain a deeper understanding of how each component of the system works.

### Week 2 Update
We learned about the strcuture of the storage system and how we can scale and add dimensions to it. We discussed about different ways of serialization, encoding, and decoding. It is helpful to build our data model. We reviewed lambda function which is very handy and useful. We learned about what entities stand for and how we can use the concept in our database system. We extended the functionality of the tokenizer to read database commands. In addition, we implemented a basic level of BlockIO to read and write blocks of information.

### Week 3 Update
We learned a lot about the design of the database, including how components communciate with each other and how each component does its job. I figured that the command processor is not a controller itself, but rather a temporary objects that constructs a corresponding statement object that tells the correct controller what to do. In our current implementation, the only two controller are application and database, and we will have three command processor: application, DBProcessor, and SQL Processor. Another important thing we learned is how to parse effectively. The parsing diagram is very eye-opening and inspiring.

### Week 4 Update
Our understanding of the database is being reinforced. Our group is working hard on making the database more robust, such as using unique pointers, keep const keyword consistent, make more input validation, etc. We are really working hard to not only have a working database but a reliable and a robust database. I have learned and thought more about advanced parsing for the insert statement and select statement in the future. Reusability and system design are the core of our thinking now. We are working hard to not fall back to linear code, instead we are trying all sorts of ways to make our database readable and efficient.

### Week 5 Update
We are delving more into retrieving and filtering data. In the filter class, we need to implement our expression class and evaluate logical expressions. We are also creating a query class which will handle the select statement. These additions make our system design better. Also we went through code better sessions to improve our code and avoid common mistakes like insufficient validation of results.

### Week 6 Update
We are focusing on abstractions we have built in the database project. There are so many designs that remain timeless regardless how many features we are adding. For example, the recognize function is an abstraction. We can add more command processor to recognize more types of input commands, but the recognize function remains the same. The statement is also a abstraction parallel to the recognize function. Storable is a great abstraction. I have realized that by defining a set of shared attribtues, we can create great abstraction. Interface is more related to verbs, and inheritance is more related to structures. Interface gives flexbility because the verbs are the expressive actions. Inheritance gives great structure for abstraction to work coherently.

### Week 7 Update
We are implementing delete rows, update rows, and drop table. To be honest these are very easy given what we have established in the select rows functionality. But I am really thinking hard about joins. Joins, indexes, and caching feel very challenging, so I am trying to get a head start at least on joins. I am thinking about how different parts of the system interact and what interface we need. In the first iteration, I want something really dumb and slow to work, kind of like select rows by brute force. It can be O(n^3), does not matter. Then we can substitute it with more efficient algorihtms, while keeping the same interface. For me, implementing something with brute force method while developing the interface for future improvement is very intuitive.

### Week 8 Update
Our team is working hard on the joins. We are thinking different ways we can make the joins efficient. But I will first talk about the side assignment given to us this week which is to run a script file. The idea is to add run_kw to a list of recognizable keywords by the application and it would search the file and use the stream as input for doScriptTest. By integrating into the database workflow directly, the benefit is that we can chain command calls across files. In the picture shown, I am running file1, and inside of file1, it is making a call to run file2. This makes the script testing very flexbile.

### Week 9 Update
We have more thoughts about a effective cache system and the implementation of an index system is taking most of our time. We are thinking about questions like which object should have the responsibilities to handle index and cache. where, what, why are the big questions we are asking ourselves. We continue to make our system more scalable and we always ask what would happen if there are a million records inserted.

### Week 10 Update
We are finishing the version 1.0 of our database. We are adding a cache system in BlockIO, Storage, and Database, for block cache, row cache, and view cache respectively. We have summarized what we have learned and how much we have grown in these 10 weeks and I am sure it will continue to benefit our career in unforseenable ways.
