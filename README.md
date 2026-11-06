SPY Stock Price Analyzer (C++)

A command-line C++ program that loads 21 years of daily SPY (S&P 500 ETF) price history from a CSV file and computes the core statistics used to describe an asset's return and risk: daily returns, average return, compounded growth, volatility and maximum drawdown.

Built as a C++ ramp-up project, with every statistic checked against a hand calculation or an independent cross-check.

Features
Function	What it computes
loadData	Parses the CSV (Date, Open, High, Low, Close, Volume) into a vector of bars
profit	Return between any two trading dates
dailyReturns	Close-to-close daily returns in a single O(n) pass
avgReturns	Arithmetic mean daily return
compoundCheck	Total compounded growth across all daily returns
volatility	Sample standard deviation of daily returns (n − 1)
avgWindow	N-day moving average of closes for a given date, using only past data
maxdrawdown	Largest peak-to-trough decline, with peak and trough dates
Results (SPY, 2005-02-25 to 2026-10-09)
Metric	Value
Trading days loaded	5,439
Daily returns	5,438
Average daily return	0.0461%
Compounded growth	8.31× ($93.69 → $778.57)
Daily volatility	1.19%
Annualized volatility	18.9%
Max drawdown	−56.5% (peak $120.73 on 2007-10-09, trough on 2009-03-09)
20-day moving average, 2005-03-24	92.7233
Verification

Each result was checked independently rather than trusted on sight:

Loading: row count, first bar and last bar compared against the raw file.
Returns: the first daily return (−0.68%) matches a hand calculation, and the profit function gives the same value for the same two dates.
Compounding: multiplying all (1 + r) values gives 8.30964, matching the direct price ratio 778.57 / 93.6948.
Volatility: agrees with the volatility-drag relation geometric mean ≈ arithmetic mean − σ²/2. The gap between the arithmetic and geometric daily means (≈ 0.0000712) matches σ²/2 (≈ 0.0000711).
Max drawdown: the trough close (52.5498) checked by hand: (120.73 − 52.5498) / 120.73 = 0.5647.
Moving average boundary: with a 20-day window, index 18 is correctly rejected and index 19, the first date with full history, is accepted.
Implementation notes
No lookahead bias: the moving average uses the given day and the N − 1 days before it, never future prices.
Linear time: daily returns are computed directly from adjacent bars rather than by searching for dates, which avoids an O(n²) approach.
Sample standard deviation: volatility divides by n − 1, since the data is a sample of SPY's history.
Annualization: daily volatility is scaled by √252, because variance (not standard deviation) grows linearly with time.
Separation of concerns: maxdrawdown returns a struct of raw values; formatting is left to the caller.
Data

Daily SPY history from Stooq (symbol spy.us), saved as spy_us_d.csv in the project folder. Format:

Date,Open,High,Low,Close,Volume
2005-02-25,92.7949,93.8716,92.7097,93.6948,79289004

Prices are adjusted for dividends, so historical values are lower than the prices SPY actually traded at, and returns include dividends.

Build and run

Requires a C++20 compiler.

g++ -std=c++20 -Wall -Wextra main.cpp -o main
./main

Run it from the folder that contains spy_us_d.csv, since the file is opened by relative path.

Possible extensions
Split into multiple source files with a CMake build
Load several tickers into a map and compare their statistics
Unit tests on a small hand-made CSV with known answers
Rolling moving-average series using an O(n) sliding-window sum
Replay the CSV bar by bar to simulate a live data stream
