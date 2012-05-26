-- demo level

set_generate_speed(400)
max_creatures(player0,12)
start_money(player0,5000)

add_creature_to_pool(fly,4)
add_creature_to_pool(bug,7)
add_creature_to_pool(spider,3)
add_creature_to_pool(troll,5)
add_creature_to_pool(bile_demon,3)

creature_available(player0,fly,1,0)
creature_available(player0,bug,1,0)
creature_available(player0,spider,1,0)
creature_available(player0,troll,1,0)
creature_available(player0,bile_demon,1,0)

room_available(player0,treasure,1,0)
room_available(player0,garden,1,0)
room_available(player0,lair,1,0)
room_available(player0,training,1,0)
room_available(player0,bridge,1,0)

magic_available(player0,power_hand,1,1)
magic_available(player0,power_slap,1,1)
magic_available(player0,power_imp,1,1)
magic_available(player0,power_speed,1,0)
magic_available(player0,power_heal_creature,1,0)


create_party(one)
	add_to_party(one,thief,1,300,attack_enemies,0)
	add_to_party(one,thief,1,300,attack_enemies,0)
	add_to_party(one,thief,1,300,attack_enemies,0)

create_party(victor)
        add_to_party(victor,archer,2,600,attack_enemies,0)
        add_to_party(victor,archer,1,400,attack_enemies,0)
        add_to_party(victor,archer,1,400,attack_enemies,0)
        add_to_party(victor,wizard,2,800,attack_enemies,0)

create_party(final)
	add_to_party(final,wizard,2,800,attack_enemies,0)
	add_to_party(final,archer,2,500,attack_enemies,0)
	add_to_party(final,archer,2,500,attack_enemies,0)
	add_to_party(final,knight,3,2000,attack_enemies,0)

if (( get_value(player0,game_turn ) >= 40) ) then
	quick_objective(1,"Money sits at the root of all Evil, and so it is appropriate that you start your Empire by building a Treasure Room. Select the Treasure Room Icon with the Left Mouse Button (LMB) and build a 3X3 Treasure Room in an empty space in your Dungeon.",player0)
	room_available(player0,treasure,1,1)
	tutorial_flash_button(6,-1)
end


if (( get_value(player0, game_turn ) >= 100) ) then
        display_information(1,player0)
end


if (( get_value(player0, game_turn ) >= 450) ) then
        quick_information(33, "You may see a map of the entire realm by pressing 'M' or clicking on the scroll at the top-left of your panel.")
end



if (( get_value(player0,treasure ) >= 1) ) then
        quick_information(30, "A Treasure Room is used to store any gold mined by your imps.")
        if (( get_value(player0,money ) <= 1000) ) then
                quick_information(2,"Your gold reserves are running low. Tag the gold seam to the South. Your imps will mine it out and replenish your coffers.")
        end
end



if (( get_value(player0,treasure ) >= 9) ) then
	quick_objective(3,"Click with the right mouse button to stop building rooms. Now tunnel to the Portal in the East by tagging the rock with your left mouse button. Your Imps will then claim it for you.",player0)
	set_timer(player2,timer0)
	set_flag(player0,flag4,1)
        if (( get_value(player2,timer0 ) >= 275) ) then
                quick_information(7,"Using the right mouse button (RMB), you may slap your creatures, thus making them work more quickly. Be careful not to beat them too frequently! This will make them angry, or at worst you will beat them to death!")
                if (( get_value(player2,timer0 ) >= 550) ) then
                        quick_information(8,"Clicking with the LMB whilst over a minion will pick that creature up. Any eight of the creatures you own can be carried in your hand like this. Use this method to place Imps next to the tasks that you wish them to carry out. Drop them on your own, or Neutral territory with the RMB.") 
                end
        end
end

if (( get_value(player0,flag4 ) == 1) ) then
	if (( get_value(player0,entrance ) >= 1) ) then
		room_available(player0,lair,1,1)
		tutorial_flash_button(8,-1)
        quick_objective(19,"Now you have claimed a portal creatures will enter your dungeon seeking employment. You must build a 3X3 Lair to house your creatures. Larger Lairs hold more creatures.",player0)
	end
end

if (( get_value(player0,lair ) >= 9) ) then
        quick_objective(31,"Now you have built a small lair, await the arrival of your first minion.",player0)
	set_timer(player2,timer1)
		if (( get_value(player2,timer1 ) >= 250) ) then
                quick_information(20,"Rooms become more efficient as they increase in size. Build neat, rectangular rooms to ensure maximum efficiency.")
		creature_available(player0,fly,1,1)
		end
end

if (( get_value(player0,fly ) >= 1) ) then
        quick_objective(4,"A humble Fly has become the first of your troops. More powerful creatures require a source of sustenance. Build a 3X3 Hatchery to feed your creatures.",player0)
	room_available(player0,garden,1,1)
	tutorial_flash_button(7,-1)
end

if (( get_value(player0,bug ) >= 1) ) then
        quick_information(5,"A Beetle has settled in your Lair. To make your dungeon more appealing to the creatures of this world you can build bigger rooms.")
end

if (( get_value(player0,garden ) >= 9) ) then
        set_timer(player0,timer1)
        creature_available(player0,bug,1,1)

        quick_objective(23,"While you are waiting for your ranks to grow, try possessing a creature. Select the icon from the spell menu and choose your creature, right clicking to release it from its bond.", player0)
        tutorial_flash_button(36,-1)
end



if (( get_value(player0,total_creatures ) >= 5) ) then
        quick_information(32,"Left Click on a creature in the creature panel to place it in your Hand of Evil. Clicking on the creature icon itself will select the most experienced specimen of the breed.")
end

if (( get_value(player0,total_creatures ) >= 7) ) then
	room_available(player0,training,1,1)
	tutorial_flash_button(10,-1)
	creature_available(player0,troll,1,1)
	creature_available(player0,spider,1,1)
	creature_available(player0,bile_demon,1,1)
        quick_objective(6,"Now dig out a 3X3 area and, once your imps have claimed the floor, build a Training Room in the space you have excavated.",player0)
end

if (( get_value(player0,training ) >= 1) ) then
        quick_information(34,"Creatures in the training room will improve their fighting prowess.")

        if (( get_value(player0,training ) >= 9) ) then
                set_timer(player0,timer2)
                quick_objective(35,"Now that you have built the training room, you can tell your creatures to train by picking them up with the LMB and dropping them in it.",player0)

                if (( get_value(player0,timer2 ) >= 800) ) then
					if (( get_value(player0, flag2 ) == 0) ) then
	                        quick_objective(18,"You may create more imps by choosing the Create Imp spell in the spell panel and then clicking with the left mouse button on a section of your dungeon floor.",player0)
							set_flag(player0, flag2, 1)
					end
                        tutorial_flash_button(21,-1)
                        set_flag(player_good,flag0,1)

                        if (( get_value(player0,timer2 ) >= 1200) ) then
                                quick_objective(9,"Your mission here is to use your power to rid this land of all good. Forge a path North!",player0)
                        end
                 end
         end
end
                           
if ( action_point(1,player0) ) then
        quick_information(10,"You have found a spell of acceleration. When cast on your creatures this will allow them to move at a greatly increased rate for a time. Holding down the LMB will increase the power and cost of the spell.")
	magic_available(player0,power_speed,1,1)
end

if ( action_point(2,player0) ) then
	if (( get_value(player_good,thief ) == 0) ) then
        quick_objective(11,"The Heroes' castle lies out of reach on the far bank of the lava flow. Use bridges to connect your land to the north bank and seek out this nest of virtue.",player0)
        tutorial_flash_button(18, -1)
	room_available(player0,bridge,1,1)
        set_timer(player_good,timer1)
	end
end


if ( action_point(5,player0) ) then
	if (( get_value(player0,bridge ) >= 3) ) then
	magic_available(player0,power_heal_creature,1,1)
	quick_information(15,"You now have the ability to cast Heal on your creatures. Hold down the LMB for more power. The hero forces are sure to retaliate")
	end
end

if (( get_value(player0,troll ) >= 1) ) then
	quick_information(21,"A goblin has entered your dungeon. While not as intelligent as the average human, goblins make excellent soldiers.")
end

if (( get_value(player0,bile_demon ) >= 1) ) then
        quick_information(22,"You have found a new Bile Demon ally. The aggression and tenacity of these beasts is matched only by their appetites.")
end

if ( action_point(7,player0) ) then
	if (( get_value(player_good,flag0 ) == 1) ) then
                quick_objective(26,"From the markings, you have broken into an old dungeon. Claim any assets --aining, but be on your guard!",player0)
                add_tunneller_party_to_level(player_good,one,-1,dungeon,0,2,600)
	end
end

if ( action_point(7,player0) ) then
	if (( get_value(player_good,flag0 ) == 0) ) then
		add_party_to_level(player_good,victor,7,1)
                quick_information(28,"Danger! Your untidy exploration of this realm has left you exposed to the Lord's troops.")
		set_timer(player2,timer3)
	end
end

if (( get_value(player2,timer3 ) >= 350) ) then
	if (( get_value(player_good,flag0 ) == 0) ) then
		next_command_reusable()
		add_party_to_level(player_good,victor,7,1)
		next_command_reusable()
		set_timer(player2,timer3)
	end
end

if ( action_point(3,player0) ) then
        quick_objective(37,"You've discovered the Heroes' castle. Break down the doors and wreak havoc upon the enemy!",player0)
        add_party_to_level(player_good,final,player_good,1)
	set_flag(player0,flag1,1)
end

if ( action_point(5,player0) ) then
       add_party_to_level(player_good,victor,-4,1)
end

if (( get_value(player0, flag1 ) == 1) ) then
	if (( get_value(player_good, knight ) == 0) ) then
                 quick_objective(36,"Congratulations. You have defeated the Lord of the Land and completed the Dungeon Keeper demo.",player0)
		win_game()
	end
end

if (( get_value(player0,total_imps ) <= 3) ) then
	if (( get_value(player0, flag2 ) == 0) ) then
        	quick_information(39,"You may create more imps by choosing the Create Imp spell in the spell panel and then clicking with the left mouse button on a section of your dungeon floor.",player0)
		set_flag(player0, flag2, 1)
	end
end

if (( get_value(player0,dungeon_destroyed ) == 1) ) then
	lose_game()
end












				










   
