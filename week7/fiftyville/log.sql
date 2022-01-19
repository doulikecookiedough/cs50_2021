-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Known Data --
-- Theft Date: July 28, 2020
-- Theft Location: Chamberlin Street

-- Get further information from crime_scene_reports
SELECT description
FROM crime_scene_reports
WHERE street = 'Chamberlin Street';
-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
-- ... Interviews were conducted today with three witnesses who were present at the time
-- ... — each of their interview transcripts mentions the courthouse.

-- Review interviews
SELECT transcript
FROM interviews
WHERE transcript LIKE "%courthouse%";
-- Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away.
-- .. If you have security footage from the courthouse parking lot, you might want to look for cars that left the
-- .. parking lot in that time frame.
-- I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse,
-- .. I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
-- As the thief was leaving the courthouse, they called someone who talked to them for less than a minute.
-- .. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- .. The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Theft @ 10:15 - 10:25, Review License Plates
SELECT license_plate
FROM courthouse_security_logs
WHERE minute BETWEEN 15 AND 25
AND year = 2020
AND month = 7
AND day = 28
AND hour = 10;
-- 5P2BI95, 94KL13X, 6P58WS2, 4328GD8, G412CB7, L93JTIZ, 322W7JE, 0NTHK55

-- Review names of all people from license plates provided
SELECT name, passport_number
FROM people
WHERE license_plate IN ('5P2BI95', '94KL13X', '6P58WS2', '4328GD8', 'G412CB7', 'L93JTIZ', '22W7JE', '0NTHK55');
-- Patrick | 2963008352
-- Amber | 7526138472
-- Elizabeth | 7049073643
-- Roger | 1695452385
-- Danielle | 8496433585
-- Evelyn | 8294398571
-- Ernest | 5773159633

-- Find the earliest flight out of Fiftyville on July 29, 2020
SELECT id, hour, minute
FROM flights
WHERE year = 2020
AND month = 7
AND day = 29;
-- id | hour | minute
-- 18 | 16 | 0
-- 23 | 12 | 15
-- 36 | 8 | 20
-- 43 | 9 | 30
-- 53 | 15 | 20

-- Find the passengers on the earliest flight (id: 36) that are on the list of suspected thief passports
SELECT passport_number
FROM passengers
WHERE passport_number IN ('2963008352', '7526138472', '7049073643', '1695452385', '8496433585', '8294398571', '5773159633')
AND flight_id = 36;
-- 1695452385 (Roger)
-- 5773159633 (Ernest)
-- 8294398571 (Evelyn)
-- 8496433585 (Danielle)

-- Check who made phone calls between 10:15 - 10:25 on July 28, 2020
SELECT caller, receiver
FROM phone_calls
WHERE duration < 60
AND year = 2020
AND month = 7
AND day = 28;
-- (130) 555-0289 | (996) 555-8899
-- (499) 555-9472 | (892) 555-8872
-- (367) 555-5533 | (375) 555-8161 ***
-- (499) 555-9472 | (717) 555-1342
-- (286) 555-6063 | (676) 555-6554
-- (770) 555-1861 | (725) 555-3243
-- (031) 555-6622 | (910) 555-3251
-- (826) 555-1652 | (066) 555-9701
-- (338) 555-6650 | (704) 555-2131

-- Check who withdrew money on July 28, 2020 at Fifer Street
SELECT *
FROM atm_transactions
WHERE atm_location = 'Fifer Street'
AND year = 2020
AND month = 7
AND day = 28;
-- 246 | 28500762 | 2020 | 7 | 28 | Fifer Street | withdraw | 48
-- 264 | 28296815 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
-- 266 | 76054385 | 2020 | 7 | 28 | Fifer Street | withdraw | 60
-- 267 | 49610011 | 2020 | 7 | 28 | Fifer Street | withdraw | 50
-- 269 | 16153065 | 2020 | 7 | 28 | Fifer Street | withdraw | 80
-- 275 | 86363979 | 2020 | 7 | 28 | Fifer Street | deposit | 10
-- 288 | 25506511 | 2020 | 7 | 28 | Fifer Street | withdraw | 20
-- 313 | 81061156 | 2020 | 7 | 28 | Fifer Street | withdraw | 30
-- 336 | 26013199 | 2020 | 7 | 28 | Fifer Street | withdraw | 35

-- Find names of the accounts that withdrew money
SELECT name, phone_number
FROM people
JOIN bank_accounts
ON bank_accounts.person_id = people.id
WHERE account_number IN ('28500762', '28296815', '76054385', '49610011', '16153065', '25506511', '81061156', '26013199');
-- Ernest | (367) 555-5533 ***
-- Russell | (770) 555-1861
-- Roy | (122) 555-4581
-- Bobby | (826) 555-1652
-- Elizabeth | (829) 555-5269
-- Danielle | (389) 555-5198 ***
-- Madison | (286) 555-6063
-- Victoria | (338) 555-6650

-- The thief is Ernest as his number is the only one that made a call during the theft window

-- Find his accompice with his number: (375) 555-8161
SELECT name
FROM people
WHERE phone_number = '(375) 555-8161';
-- Berthold

-- Find where Ernest escaped to
SELECT destination_airport_id
FROM flights
WHERE id = 36;
-- 4

-- Find the name of the destination
SELECT city, full_name
FROM airports
WHERE id = 4;
-- London, Heathrow Airport