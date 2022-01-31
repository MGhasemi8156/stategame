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
- [ ] define const
- [ ] error handling
### soldier:
- [x] struct Soldier
- [x] add_soldiers()
- [x] draw_soldiers()
- [x] check_for_collisions()
- [x] realloc 
- [x] check soldiers collisions to work properly 
- [x] move in paralel lines
- [x] collision detection for potions
- [ ] define consts
- [ ] error handling 
### events:
- [x] event_listener()
### potions:
- [ ] create and add potions
- [x] create stop_enemy potion
- [ ] create increase velocity potion
- [ ] create no limit birth potions
- [ ] create increse rebirth rate potion  
- [ ] make specefic shape and color for potoins
- [ ] define consts
- [ ] error handling
### ai:
- [ ] make a good one
### design:
- [ ] initial number of soldiers

