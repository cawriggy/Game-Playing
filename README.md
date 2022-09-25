# Creating opponents for 2-player zero-sum games

- Noughts and Crosses
- Connect 4
- Mancala

### 1. Techniques
- Random actions
- Minimax
- Alpha-beta pruning

Randomly selected actions are quick but often not effective. They can be useful for testing.

Minimax can play optimally according to some score function if you can let it take the time to search to the end of the game. More commonly the search depth is restricted.   https://en.wikipedia.org/wiki/Minimax

Alpha-beta pruning can be used to skip some irrelevant branches of the search tree when calculating the minimax value. e.g. if you found that you can force a draw you don't need to search to the end of a branch where the opponent has the option to make you lose.
https://en.wikipedia.org/wiki/Alpha-beta_pruning
https://www.chessprogramming.org/Alpha-Beta

### 2. Scoring
For Noughts and Crosses and Connect 4, a score function was used to evaluate the terminal state. If the depth limit was reached the state was scored as a draw. (Random playouts were not found to be effective for these games.)

The score for Mancala was evaluated at any state using the number of stones in the mancalas. Long term bounds on the score function were used to achieve earlier cutoffs with Alpha-beta pruning. Eg don't search a branch where there are only 9 stones remaining but 10 are needed to beat the best alternative. 
A heuristic score evaluated nearer the end of the game is a more accurate reflection of the final outcome. So deeper searches tend to give better play and play was better towards the end of the game.

It is important to ensure the score function aligns with the intended selections. eg it may be preferable to favour losing in 10 turns over losing in 1 turn because the opponent may be sub optimal and make a mistake. If the score function only considers wins/losses/draws, a Minimax player has no reason to avoid terrible moves if its opponent can already force a  win. Even if the win is many moves in the future. For this reason a deeper searching Minimax player can actually play worse. This was seen in a Noughts and Crosses scenario when it didn't bother blocking either of 2 winning options for the opponent.

### 3. Move ordering
The speed of Alpha-beta pruning is improved if good options are searched earlier since this allows more cutoffs (by raising the minimum score sooner).
For example in this implementation, reversing the move ordering for Mancala reduced the time taken by 80%. (The time to play 100 games of random player vs alphabeta_mancala depth 8 decreased from 3s to 0.6s) 
A Monte Carlo tree search was then implemented to identify a good order of moves to search.

### 4. Performance
To search deeper you need to evaluate states faster or evaluate less states. To evaluate fewer states, caching was used with Minimax to reuse previous results and better move ordering was used for alphabeta. To traverse game states faster the games were optimised eg by using bitwise computation for Connect 4, and avoiding allocating vectors where possible for Mancala.

### 5. Important considerations for creating AI opponents
Any states that are given the same value will be chosen between arbitrarily. If the score function is not chosen wisely and a perfect opponent could force a win then a Minimax player won't make it difficult for a weak opponent to win. More generally it will not try to make it difficult for a weak player to do as well as a strong player. This is likely to be undesirable in an AI player. To avoid this a more fine grained score function is needed - eg for Connect 4 this was addressed by making winning early and losing later better. For Mancala the margin of the win was used. 



## How to run 
- Open the solution file in Visual Studio 2022.
- Hit F5 

You can change the search depth or play interactively by editing Game-Playing.cpp
