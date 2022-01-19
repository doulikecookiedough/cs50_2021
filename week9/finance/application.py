import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

# # Start of Personal Touch - Password Validation
# from password_validator import PasswordValidator

# passwordschema = PasswordValidator()
# passwordschema\
# .min(8)\
# .max(100)\
# .has().uppercase()\
# .has().lowercase()\
# .has().digits()\
# .has().no().spaces()\
# # End of Personal Touch - Password Validation

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Get user's current cash total
    usercashdata = db.execute("SELECT cash FROM users WHERE id=:id",
        id=session["user_id"])
    currentcash = usercashdata[0]['cash']
    grandtotal = currentcash
    portfoliototal = 0

    # Get user's portfolio
    portfolio = db.execute("SELECT * FROM portfolio WHERE user_id=:id",
        id=session["user_id"])

    # Calculate current value after retrieving latest prices
    for stock in portfolio:
        latestprice = lookup(stock['stock'])['price']
        total = stock['quantity'] * latestprice
        prettystocktotal = "${:,.2f}".format(total)
        stock.update({'price': latestprice, 'total': prettystocktotal})
        grandtotal += total
        portfoliototal += total

    # Format cash for easy viewing
    prettifygrandtotal = "${:,.2f}".format(grandtotal)
    prettifycurrentcash = "${:,.2f}".format(currentcash)
    prettifyportfoliototal = "${:,.2f}".format(portfoliototal)

    return render_template("index.html", stocks=portfolio, cash=prettifycurrentcash, portfoliototal=prettifyportfoliototal, grandtotal=prettifygrandtotal)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        # Check to ensure stock (symbol) is not blank
        if not request.form.get("symbol"):
            return apology("Please input stock", 400)

        # Check to ensure shares is not blank
        if not request.form.get("shares"):
            return apology("Please input shares", 400)

        # Check to ensure positive value of shares requested
        if int(request.form.get("shares")) <= 0:
            return apology("Minimum of 1 share is required to use CS50 Finance", 400)

        # Get symbol from form
        stocksymbol = request.form.get("symbol").upper()
        # Search symbol and wait for response
        stockdata = lookup(stocksymbol)

        # Handle buy
        ## Check to make sure stock exists
        if stockdata is None:
            return apology("Stock data is unavailable at this time", 400)

        # If it exists, purchase stock
        ## Calculate total cost of purchase
        shares = int(request.form.get("shares"))
        totalcost = shares * stockdata['price']
        ## Identify how much cash the user has
        cashavailable = db.execute("SELECT cash FROM users WHERE id=:id",
            id=session["user_id"])
        ## If insufficient funds, return apology
        if totalcost > cashavailable[0]["cash"]:
            return apology("Insufficient funds", 400)

        ## Update user's funds after purchase
        db.execute("UPDATE users SET cash=cash-:cost WHERE id=:id",
            cost=totalcost,
            id=session["user_id"]);

        ## Keep track of the transaction
        db.execute("INSERT INTO transactions (user_id, stock, quantity, price, date) VALUES (:user_id, :stock, :quantity, :price, :date)",
            user_id=session["user_id"],
            stock=stockdata["symbol"],
            quantity=shares,
            price=stockdata['price'],
            date=datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

        # Update user's portfolio
        ## Retrieve shares of symbol in current portfolio
        currentportfolio = db.execute("SELECT quantity FROM portfolio WHERE stock=:stock AND user_id=:user_id",
            stock=stocksymbol,
            user_id=session["user_id"])

        ## Add to portfolio if the symbol is new
        if not currentportfolio:
            db.execute("INSERT INTO portfolio (user_id, stock, quantity) VALUES (:user_id, :stock, :quantity)",
                user_id=session["user_id"],
                stock=stockdata["symbol"],
                quantity=shares)

        ## If found, update quantity/total of shares
        else:
            db.execute("UPDATE portfolio SET quantity=quantity+:quantity WHERE stock=:stock AND user_id=:user_id",
                user_id=session["user_id"],
                quantity=shares,
                stock=stocksymbol)

        # When purchase completed, redirect user back to the index page
        return redirect("/")

    else:
        return render_template("buy.html")
    # return apology("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get user's portfolio
    transactions = db.execute("SELECT * FROM transactions WHERE user_id=:id",
        id=session["user_id"])

    # Calculate current value after retrieving latest prices
    for transaction in transactions:
        # Stocks Symbol
        transactionsymbol = transaction['stock']
        # Purchase or Sale price
        transactionvalue = transaction['price']
        # No. of Shares Bought or Sold
        transactionquantity = transaction['quantity']
        # Date and time
        transactiondate = transaction['date']
        # Buy or Sell
        if transactionquantity < 0:
            transaction['status'] = 'Sell'
        else:
            transaction['status'] = 'Buy'

    # Render history template
    return render_template("history.html", transactions=transactions)



@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?",
            request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"],
            request.form.get("password")):
                return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        # Make sure there is a stock symbol submitted
        if not request.form.get("symbol"):
            return apology("please provide a stock symbol", 400)

        # Get symbol from form
        stocksymbol = request.form.get("symbol").upper()
        # Search symbol and wait for response
        stockdata = lookup(stocksymbol)

        # Handle search
        if stockdata is None:
            return apology("invalid symbol", 400)
        return render_template("quoted.html", stockdata={
            'name': stockdata['name'],
            'price': usd(stockdata['price']),
            'symbol': stockdata['symbol'],
        })

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password was submitted
        elif not request.form.get("confirmation"):
            return apology("please confirm password", 400)

        # Validate password
        ## Get password from form
        password = request.form.get("password")
        ## Get confirmation
        confirmPassword = request.form.get("confirmation")

        if password != confirmPassword:
            return apology("the passwords do not match, please try again", 400)

        # validatePassword = passwordschema.validate(password)
        # if validatePassword != True:
        #     return apology("Please ensure your password is at least 8 characters, contain an uppercase letter, a lowercase letter, a digit and no spaces.")

        # Confirm that user name has not been used
        ## Query database for username
        checkuser = db.execute("SELECT * FROM users WHERE username = ?",
            request.form.get("username"))

        # If the user does not exist, register the user
        if len(checkuser) < 1:
            db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                        username=request.form.get("username"),
                        hash=generate_password_hash(request.form.get("password")))
            return render_template("login.html")
        # If it exists, inform the user that it does
        else:
            return apology("username already exists", 400)

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Check to ensure stock (symbol) is not blank
        if not request.form.get("symbol"):
            return apology("Please input stock", 400)

        # Check to ensure shares is not blank
        if not request.form.get("shares"):
            return apology("Please input shares", 400)

        # Check to ensure positive value of shares requested
        if int(request.form.get("shares")) < 0:
            return apology("Minimum of 1 share is required to use CS50 Finance", 400)

        # Get stock user has requested to sell
        stocktosell = request.form.get("symbol")
        sharestosell = int(request.form.get("shares"))

        # Retrieve the stock data from portfolio
        stockavailability = db.execute("SELECT quantity FROM portfolio WHERE stock=:stock AND user_id=:user_id",
            user_id=session["user_id"],
            stock=stocktosell)

        # If stock does not exist, return apology
        if not stockavailability:
            return apology("You do not have any available shares to sell", 400)

        # Determine if user has required stock to sell, kick out if invalid
        if sharestosell > stockavailability[0]['quantity']:
            return apology("You do not have enough shares to sell")

        # Get the latest stock data
        stockdata = lookup(stocktosell)

        # Check to make sure stock exists
        if stockdata is None:
            return apology("Stock data is unavailable at this time", 400)

        # If it exists, proceed to sell
        sellvalue = int(sharestosell) * stockdata['price']

        # Update databases
        ## Start with cash
        db.execute("UPDATE users SET cash=cash+:cost WHERE id=:id",
            cost=sellvalue,
            id=session["user_id"]);
        ## Now add transaction to database
        db.execute("INSERT INTO transactions (user_id, stock, quantity, price, date) VALUES (:user_id, :stock, :quantity, :price, :date)",
            user_id=session["user_id"],
            stock=stockdata["symbol"],
            quantity=-sharestosell,
            price=stockdata['price'],
            date=datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        ## Now update portfolio
        db.execute("UPDATE portfolio SET quantity=quantity-:quantity WHERE stock=:stock AND user_id=:user_id",
            user_id=session["user_id"],
            quantity=sharestosell,
            stock=stockdata["symbol"])

        # Return user to index
        return redirect("/")

    else:
        # Grab all available shares to sell
        currentportfolio = db.execute("SELECT stock FROM portfolio WHERE user_id=:user_id",
            user_id=session["user_id"])
        return render_template("sell.html", stocks=currentportfolio)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
