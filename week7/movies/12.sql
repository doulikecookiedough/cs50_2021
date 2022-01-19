-- In 12.sql, write a SQL query to list the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred.
-- Your query should output a table with a single column for the title of each movie.
-- You may assume that there is only one person in the database with the name Johnny Depp.
-- You may assume that there is only one person in the database with the name Helena Bonham Carter.

-- CREATE INDEX person_index ON stars (person_id);
-- CREATE INDEX movie_index ON stars (movie_id);
-- CREATE INDEX name_index ON people (name);

SELECT title
FROM movies
JOIN stars
ON stars.movie_id = movies.id
JOIN people
ON people.id = stars.person_id
WHERE name = 'Johnny Depp'
INTERSECT
SELECT title
FROM movies
JOIN stars
ON stars.movie_id = movies.id
JOIN people
ON people.id = stars.person_id
WHERE name = 'Helena Bonham Carter';