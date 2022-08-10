# Game-Playing
Exploring algorithms to play 2-player zero-sum games with the aim to create an AI player for a card game.



# Exploring game playing algorithms in Noughts and Crosses
### Structure code to enable extensions to other two player games

two player zero-sum games (i.e. one player wins and the other loses or the game is a draw)

# Algorithms
- Minimax
- Alpha-Beta pruning
- random actions

minimax can play optimally if you can let it take the time to search to the end of the game

alpha-beta pruning makes the same choices but is faster by skipping some irrelevant branches of the search tree
(if you found a that you can force a draw you don't need to search to the end of a branch where the opponent has the option to make you lose)

random actions are quick but not effective



# Minimax
maximise the minimum score an opponent must allow you
https://en.wikipedia.org/wiki/Minimax

A terminal state is a win, loss, or draw. Greater scores are given to better outcomes (win early, lose late)
Earlier states are scored by recursively scoring future states.
  You take action to maximise your score;
  your opponent takes actions to minimise it.

Considering every future state can take too long.
It can be faster to perform a depth limited search
(e.g. looking upto 3 moves ahead instead of to the end of the game)
Once the depth limit has been reached, the game states are scored approximately.
One way to do this is to take random moves until a terminal state is reached and use the score of that state. (Random moves can be chosen quickly and only one path to the end of the game is considered instead of many)

Minimax assumes both players optimally minimise and maximise the score
if an opponent can force a loss in 10 turns and losing in 10 turns has the same score as losing in 1 turn then minimax can make seemingly terrible choices (at least from the perspective of a non-optimal player)
It may be preferable to favour losing in 10 turns over losing in 1 turn. This can be done by reducing the value of future states so that future losses are less negative and future wins are less positive.

if an opponent has multiple ways of forcing the minimum score than a minmax player will pick an arbitrary action rather than try to reduce the number of optimal options for the opponent.
This is because assumes that the opponent can only execute a perfect minmax strategy. If this is not the case (e.g. when playing against a person) then a minmax player can make seemingingly terrible choices even when it is allowed to search the entire game tree.
if the opponent can limit the minmax score regardless of the players action then a minmax player will make no attempt to make it difficult to do so.
if the opponent might not follow a perfect minmax strategy then a minmax player will effectively give up too soon.

The minmax player assumes that the opponent plays perferctly.
If you can force a draw then a minmax opponent will make no attempt to win. It will take arbitrary moves that, while not allowing you to win, will not try to make it difficult for you to draw.

If an optimal player could force a win then minmax will not resist.

# Alpha-beta pruning
Makes minimax faster without changing the result
https://en.wikipedia.org/wiki/Alpha-beta_pruning

skip branches that cannot affect the result
(when the current branch is too good for your opponent to allow or too bad for you to choose, i.e. either player prefer another option they already saw)
alpha is the minimum score you are willing to accept
beta is the maximum score your opponent is willing to accept
alpha and beta are updated as more preferable options are found so a player will not accept anything worse than their current best option

