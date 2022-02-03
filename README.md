# Conquer states
A game based on [state.io](https://play.google.com/store/apps/details?id=io.state.fight&hl=fa&gl=US) created with C and SDL2.

## build
building instructions ... .
### Mohsen Ghasemi
### 400105166

## TODO
### map:
- [x] struct Land
- [x] load_map() => make array of Lands
- [x] apply_map() => render and apply changes each frame
- [ ] make text number better
- [x] make_random_map() => make array of Lands
- [x] make some sample maps
- [x] draw attack line
- [x] make attack line a streaming pattern
- [x] rebirth
- [x] make new file for making a random map
- [ ] check segment fault for 32 lands
- [ ] set coordinates better
- [ ] add a streaming sea at background 
- [x] save rebirth stuff in .txt file
- [ ] save game
- [x] take screen shot of new map
- [ ] win checker 
- [ ] lose checker    
- [x] define const
- [x] error handling
### soldier:
- [x] struct Soldier
- [x] add_soldiers()
- [x] draw_soldiers()
- [x] check_for_collisions()
- [x] realloc 
- [x] check soldiers collisions to work properly 
- [x] move in paralel lines
- [x] collision detection for potions
- [x] define consts
- [x] error handling 
### events:
- [x] event_listener()
### potions:
- [x] create and add potions
- [x] create stop_enemy potion
- [x] create increase velocity potion
- [x] create no limit birth potions
- [x] create increse rebirth rate potion  
- [x] make specefic shape and color for potoins
- [x] add potions timer 
- [x] define consts
- [x] error handling
- [ ] make text number better
###ai:
- [ ] make a good one
### design:
- [ ] initial number of soldiers
### ui:
- [ ] start meue with input form and 3 buttons
- [ ] continue last game
- [ ] new game
- [ ] scoreboard
