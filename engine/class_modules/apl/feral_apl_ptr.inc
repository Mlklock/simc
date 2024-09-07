action_priority_list_t* precombat = get_action_priority_list( "precombat" );
action_priority_list_t* def = get_action_priority_list( "default" );
action_priority_list_t* aoe_builder = get_action_priority_list( "aoe_builder" );
action_priority_list_t* berserk = get_action_priority_list( "berserk" );
action_priority_list_t* builder = get_action_priority_list( "builder" );
action_priority_list_t* cooldown = get_action_priority_list( "cooldown" );
action_priority_list_t* finisher = get_action_priority_list( "finisher" );
action_priority_list_t* variable = get_action_priority_list( "variable" );

precombat->add_action( "cat_form,if=!buff.cat_form.up" );
precombat->add_action( "prowl,if=!buff.prowl.up" );
precombat->add_action( "variable,name=trinket_1_buffs,value=trinket.1.has_use_buff","check if trinket slot contains a stat on use (variable is just kept in case of things like mirror not working with has_use_buff)" );
precombat->add_action( "variable,name=trinket_2_buffs,value=trinket.2.has_use_buff" );
precombat->add_action( "variable,name=trinket_1_sync,op=setif,value=1,value_else=0.5,condition=talent.convoke_the_spirits&!talent.ashamanes_guidance&variable.trinket_1_buffs&(trinket.1.cooldown.duration%%120=0|120%%trinket.1.cooldown.duration=0)","if we are playing 2 minute convoke, we prefer 2 minute on-use over 3 minute on-use even without berserk: heart of the lion" );
precombat->add_action( "variable,name=trinket_2_sync,op=setif,value=1,value_else=0.5,condition=talent.convoke_the_spirits&!talent.ashamanes_guidance&variable.trinket_2_buffs&(trinket.1.cooldown.duration%%120=0|120%%trinket.1.cooldown.duration=0)" );
precombat->add_action( "variable,name=trinket_1_sync,op=setif,value=1,value_else=0.5,condition=!(talent.convoke_the_spirits&!talent.ashamanes_guidance)&variable.trinket_1_buffs&(trinket.1.cooldown.duration%%cooldown.bs_inc.duration=0|cooldown.bs_inc.duration%%trinket.1.cooldown.duration=0|trinket.1.cooldown.duration%%cooldown.convoke_the_spirits.duration=0|cooldown.convoke_the_spirits.duration%%trinket.1.cooldown.duration=0)","if we aren't playing 2 minute convoke, then we can sync 3 minute cds with berserk sans hotl" );
precombat->add_action( "variable,name=trinket_2_sync,op=setif,value=1,value_else=0.5,condition=!(talent.convoke_the_spirits&!talent.ashamanes_guidance)&variable.trinket_2_buffs&(trinket.2.cooldown.duration%%cooldown.bs_inc.duration=0|cooldown.bs_inc.duration%%trinket.2.cooldown.duration=0|trinket.2.cooldown.duration%%cooldown.convoke_the_spirits.duration=0|cooldown.convoke_the_spirits.duration%%trinket.2.cooldown.duration=0)" );
precombat->add_action( "variable,name=trinket_priority,op=setif,value=2,value_else=1,condition=!variable.trinket_1_buffs&variable.trinket_2_buffs|variable.trinket_2_buffs&((trinket.2.cooldown.duration%trinket.2.proc.any_dps.duration)*(1.5+trinket.2.has_buff.agility)*(1.2+trinket.2.has_buff.mastery)*(variable.trinket_2_sync))>((trinket.1.cooldown.duration%trinket.1.proc.any_dps.duration)*(1.5+trinket.1.has_buff.agility)*(1.2+trinket.1.has_buff.mastery)*(variable.trinket_1_sync))","prioritize trinkets that line-up with cds -> main-stat on uses -> longer cd trinkets -> shorter duration trinkets" );

def->add_action( "prowl,if=buff.bs_inc.down&!buff.prowl.up" );
def->add_action( "cat_form,if=!buff.cat_form.up&!talent.fluid_form" );
def->add_action( "invoke_external_buff,name=power_infusion,if=buff.bs_inc.up|!talent.berserk_heart_of_the_lion","Line up <a href='https://www.wowhead.com/spell=10060/power-infusion'>Power Infusion</a> with Berserk." );
def->add_action( "call_action_list,name=variable" );
def->add_action( "auto_attack,if=!buff.prowl.up|!buff.shadowmeld.up" );
def->add_action( "tigers_fury,if=(energy.deficit>35|combo_points=5)&(fight_remains<=15|(cooldown.bs_inc.remains>20&target.time_to_die>5)|(cooldown.bs_inc.ready&target.time_to_die>12|target.time_to_die=fight_remains))","actions+=/stop_moving,if=gcd.remains=0&equipped.ovinaxs_mercurial_egg" );
def->add_action( "rake,target_if=max:refreshable+(persistent_multiplier>dot.rake.pmultiplier),if=buff.shadowmeld.up|buff.prowl.up" );
def->add_action( "natures_vigil,if=spell_targets.swipe_cat>0" );
def->add_action( "renewal,if=spell_targets.swipe_cat>0" );
def->add_action( "ferocious_bite,if=buff.apex_predators_craving.up&!(variable.need_bt&active_bt_triggers=2)" );
def->add_action( "adaptive_swarm,target_if=(!dot.adaptive_swarm_damage.ticking|dot.adaptive_swarm_damage.remains<2)&dot.adaptive_swarm_damage.stack<3&!action.adaptive_swarm_damage.in_flight&!action.adaptive_swarm.in_flight&target.time_to_die>5,if=buff.cat_form.up&!talent.unbridled_swarm.enabled|spell_targets.swipe_cat=1" );
def->add_action( "adaptive_swarm,target_if=max:(1+dot.adaptive_swarm_damage.stack)*dot.adaptive_swarm_damage.stack<3*time_to_die,if=buff.cat_form.up&dot.adaptive_swarm_damage.stack<3&talent.unbridled_swarm.enabled&spell_targets.swipe_cat>1" );
def->add_action( "call_action_list,name=cooldown,if=dot.rip.ticking" );
def->add_action( "rip,if=spell_targets=1&hero_tree.wildstalker&!(talent.raging_fury&talent.veinripper)&(buff.bloodtalons.up|!talent.bloodtalons)&(dot.rip.refreshable&buff.tigers_fury.remains>10&combo_points>=3|buff.tigers_fury.remains<3.0&buff.tigers_fury.up&combo_points>=3&remains<16)","with wildstalker and not both raging fury + veinripper, low cp rips for snapshot reasons is worth" );
def->add_action( "call_action_list,name=berserk,if=buff.bs_inc.up" );
def->add_action( "call_action_list,name=finisher,if=combo_points=5" );
def->add_action( "call_action_list,name=builder,if=spell_targets.swipe_cat=1&combo_points<5&(variable.time_to_pool<=0|!variable.need_bt|variable.proccing_bt)" );
def->add_action( "call_action_list,name=aoe_builder,if=spell_targets.swipe_cat>=2&combo_points<5&(variable.time_to_pool<=0|!variable.need_bt|variable.proccing_bt)" );
def->add_action( "regrowth,if=buff.predatory_swiftness.up&variable.regrowth" );

aoe_builder->add_action( "variable,name=proccing_bt,op=set,value=variable.need_bt" );
aoe_builder->add_action( "thrash_cat,if=refreshable&!talent.thrashing_claws&!(variable.need_bt&buff.bt_thrash.up)","maintain thrash highest prio" );
aoe_builder->add_action( "brutal_slash,target_if=min:time_to_die,if=(cooldown.brutal_slash.full_recharge_time<4|time_to_die<4|raid_event.adds.remains<4)&!(variable.need_bt&buff.bt_swipe.up)","avoid capping brs charges" );
aoe_builder->add_action( "swipe_cat,if=time_to_die<4|(talent.wild_slashes&spell_targets.swipe_cat>4&!(variable.need_bt&buff.bt_swipe.up))","with wild slashes we swipe at 5+ targets over raking/moonfire" );
aoe_builder->add_action( "prowl,target_if=dot.rake.refreshable|dot.rake.pmultiplier<1.4,if=!(variable.need_bt&buff.bt_rake.up)&action.rake.ready&gcd.remains=0&!buff.sudden_ambush.up&!variable.cc_capped" );
aoe_builder->add_action( "shadowmeld,target_if=dot.rake.refreshable|dot.rake.pmultiplier<1.4,if=!(variable.need_bt&buff.bt_rake.up)&action.rake.ready&!buff.sudden_ambush.up&!buff.prowl.up&!variable.cc_capped" );
aoe_builder->add_action( "rake,target_if=refreshable,if=talent.doubleclawed_rake&!(variable.need_bt&buff.bt_rake.up)&!variable.cc_capped","dcr rake > moonfire" );
aoe_builder->add_action( "swipe_cat,if=talent.wild_slashes&spell_targets.swipe_cat>3&!(variable.need_bt&buff.bt_swipe.up)","at 4t with wild slashes, swipe is better than moonfiring" );
aoe_builder->add_action( "moonfire_cat,target_if=refreshable,if=!(variable.need_bt&buff.bt_moonfire.up)&!variable.cc_capped","li moonfire is better than non-dcr rake" );
aoe_builder->add_action( "rake,target_if=refreshable,if=!(variable.need_bt&buff.bt_rake.up)&!variable.cc_capped" );
aoe_builder->add_action( "brutal_slash,if=!(variable.need_bt&buff.bt_swipe.up)","fillers" );
aoe_builder->add_action( "swipe_cat,if=!(variable.need_bt&buff.bt_swipe.up)" );
aoe_builder->add_action( "shred,if=!buff.sudden_ambush.up&!variable.easy_swipe&!(variable.need_bt&buff.bt_shred.up)" );
aoe_builder->add_action( "thrash_cat,if=!talent.thrashing_claws&!(variable.need_bt&buff.bt_thrash.up)" );
aoe_builder->add_action( "rake,target_if=max:ticks_gained_on_refresh,if=talent.doubleclawed_rake&buff.sudden_ambush.up&variable.need_bt&buff.bt_rake.down","fallback bt actions" );
aoe_builder->add_action( "moonfire_cat,target_if=max:ticks_gained_on_refresh,if=variable.need_bt&buff.bt_moonfire.down" );
aoe_builder->add_action( "rake,target_if=max:ticks_gained_on_refresh,if=buff.sudden_ambush.up&variable.need_bt&buff.bt_rake.down" );
aoe_builder->add_action( "shred,if=variable.need_bt&buff.bt_shred.down&!variable.easy_swipe" );
aoe_builder->add_action( "rake,target_if=dot.rake.pmultiplier<1.6,if=variable.need_bt&buff.bt_rake.down" );
aoe_builder->add_action( "thrash_cat,if=variable.need_bt&buff.bt_shred.down" );

berserk->add_action( "call_action_list,name=finisher,if=combo_points=5" );
berserk->add_action( "run_action_list,name=aoe_builder,if=spell_targets.swipe_cat>=2" );
berserk->add_action( "prowl,if=!(buff.bt_rake.up&active_bt_triggers=2)&action.rake.ready&gcd.remains=0&!buff.sudden_ambush.up&(dot.rake.refreshable|dot.rake.pmultiplier<1.4)&!buff.shadowmeld.up" );
berserk->add_action( "shadowmeld,if=!(buff.bt_rake.up&active_bt_triggers=2)&action.rake.ready&!buff.sudden_ambush.up&(dot.rake.refreshable|dot.rake.pmultiplier<1.4)&!buff.prowl.up" );
berserk->add_action( "rake,if=!(buff.bt_rake.up&active_bt_triggers=2)&(dot.rake.remains<3|buff.sudden_ambush.up&persistent_multiplier>dot.rake.pmultiplier)","rake if bt doesnt need proc and rake can be upgraded. Fish for sudden ambush procs unless rake will fall off otherwise." );
berserk->add_action( "moonfire_cat,if=refreshable" );
berserk->add_action( "thrash_cat,if=!talent.thrashing_claws&refreshable" );
berserk->add_action( "shred,if=active_bt_triggers=2&buff.bt_shred.down","proc bt when an opportunity arises" );
berserk->add_action( "brutal_slash,if=active_bt_triggers=2&buff.bt_swipe.down" );
berserk->add_action( "swipe_cat,if=active_bt_triggers=2&buff.bt_swipe.down&talent.wild_slashes" );
berserk->add_action( "brutal_slash,if=cooldown.brutal_slash.charges>1&buff.bt_swipe.down","alternate brs and shred to create bt opportunities" );
berserk->add_action( "shred,if=buff.bt_shred.down" );
berserk->add_action( "brutal_slash,if=cooldown.brutal_slash.charges>1" );
berserk->add_action( "swipe_cat,if=buff.bt_swipe.down&talent.wild_slashes" );
berserk->add_action( "shred" );

builder->add_action( "variable,name=proccing_bt,op=set,value=variable.need_bt","this variable tracks whether or not we've started our bt sequence" );
builder->add_action( "shadowmeld,if=gcd=0&energy>=35&!buff.sudden_ambush.up&(dot.rake.refreshable|dot.rake.pmultiplier<1.4)*!(variable.need_bt&buff.bt_rake.up)&buff.tigers_fury.up" );
builder->add_action( "rake,if=((refreshable&persistent_multiplier>=dot.rake.pmultiplier|dot.rake.remains<3)|buff.sudden_ambush.up&persistent_multiplier>dot.rake.pmultiplier)&!(variable.need_bt&buff.bt_rake.up)","upgrade to stealth rakes, otherwise refresh in pandemic. Delay rake as long as possible if it would downgrade" );
builder->add_action( "brutal_slash,if=cooldown.brutal_slash.full_recharge_time<4&!(variable.need_bt&buff.bt_swipe.up)" );
builder->add_action( "thrash_cat,if=refreshable&!talent.thrashing_claws" );
builder->add_action( "shred,if=buff.clearcasting.react" );
builder->add_action( "moonfire_cat,if=refreshable" );
builder->add_action( "brutal_slash,if=!(variable.need_bt&buff.bt_swipe.up)" );
builder->add_action( "swipe_cat,if=talent.wild_slashes&!(variable.need_bt&buff.bt_swipe.up)" );
builder->add_action( "shred,if=!(variable.need_bt&buff.bt_shred.up)" );
builder->add_action( "swipe_cat,if=variable.need_bt&buff.bt_swipe.down" );
builder->add_action( "rake,if=variable.need_bt&buff.bt_rake.down&persistent_multiplier>=dot.rake.pmultiplier","clip rake for bt if it wont downgrade its snapshot" );
builder->add_action( "moonfire_cat,if=variable.need_bt&buff.bt_moonfire.down" );
builder->add_action( "thrash_cat,if=variable.need_bt&buff.bt_thrash.down" );

cooldown->add_action( "incarnation,if=target.time_to_die>17|target.time_to_die=fight_remains" );
cooldown->add_action( "berserk,if=buff.tigers_fury.up&(target.time_to_die>12|target.time_to_die=fight_remains)","berserk with tigers fury" );
cooldown->add_action( "berserking,if=buff.bs_inc.up" );
cooldown->add_action( "potion,if=buff.bs_inc.up|fight_remains<32|(!variable.lastZerk&variable.lastConvoke&cooldown.convoke_the_spirits.remains<10)","todo make last set of cds line up again now that we know potions.remains is the syntax" );
cooldown->add_action( "use_items","non trinket gear-on-uses have variable rules on whether or not they trigger the trinket shared CD. For the cases they do we will need specific APL entries. For now just use on cooldown." );
cooldown->add_action( "use_item,name=ovinaxs_mercurial_egg,if=!in_combat","we disable mercurial egg use here to not give the wrong impression on when to use it. (sim won't need this, could check if moving & stack is below desired threshold which isn't known yet since trinket bugged in-game)" );
cooldown->add_action( "feral_frenzy,if=combo_points<=1|buff.bs_inc.up&combo_points<=2" );
cooldown->add_action( "do_treacherous_transmitter_task,if=buff.tigers_fury.up|fight_remains<22" );
cooldown->add_action( "use_item,slot=trinket1,if=!trinket.1.is.ovinaxs_mercurial_egg&(buff.bs_inc.up|((buff.tigers_fury.up&cooldown.tigers_fury.remains>20)&(cooldown.convoke_the_spirits.remains<4|cooldown.convoke_the_spirits.remains>45|(variable.trinket_2_buffs&cooldown.convoke_the_spirits.remains-trinket.2.cooldown.remains>0)|!talent.convoke_the_spirits&(cooldown.bs_inc.remains>40|cooldown.bs_inc.remains-trinket.2.cooldown.remains>0))))&(!trinket.2.has_cooldown|trinket.2.cooldown.remains|variable.trinket_priority=1)|trinket.1.proc.any_dps.duration>=fight_remains","stat on-use trinkets, prefers trinket with higher priority. reads like this: berserk is up OR if tigers fury up & convoke is either ready, wont be for the next tf, or the other trinket will be ready in time for convoke. If we dont have convoke, then we use if berserk wont be up for next tf or other trinket will be up for berserk." );
cooldown->add_action( "use_item,slot=trinket2,if=!trinket.2.is.ovinaxs_mercurial_egg&(buff.bs_inc.up|((buff.tigers_fury.up&cooldown.tigers_fury.remains>20)&(cooldown.convoke_the_spirits.remains<4|cooldown.convoke_the_spirits.remains>45|(variable.trinket_1_buffs&cooldown.convoke_the_spirits.remains-trinket.1.cooldown.remains>0)|!talent.convoke_the_spirits&(cooldown.bs_inc.remains>40|cooldown.bs_inc.remains-trinket.1.cooldown.remains>0))))&(!trinket.1.has_cooldown|trinket.1.cooldown.remains|variable.trinket_priority=2)|trinket.2.proc.any_dps.duration>=fight_remains" );
cooldown->add_action( "use_item,slot=trinket1,if=!trinket.1.is.ovinaxs_mercurial_egg&!variable.trinket_1_buffs&(trinket.2.cooldown.remains>20|!variable.trinket_2_buffs|trinket.2.cooldown.remains&cooldown.tigers_fury.remains>20)","non-stat on use trinkets get used on cooldown, so long as it wont interfere with a stat on-use trinket" );
cooldown->add_action( "use_item,slot=trinket2,if=!trinket.2.is.ovinaxs_mercurial_egg&!variable.trinket_2_buffs&(trinket.1.cooldown.remains>20|!variable.trinket_1_buffs|trinket.1.cooldown.remains&cooldown.tigers_fury.remains>20)" );
cooldown->add_action( "convoke_the_spirits,if=fight_remains<5|(buff.bs_inc.up|!talent.berserk_heart_of_the_lion)&(buff.tigers_fury.up&(combo_points<=2|buff.bs_inc.up&combo_points<=3)&(target.time_to_die>5-talent.ashamanes_guidance.enabled|target.time_to_die=fight_remains))","always line up convoke with berserk if you can" );

finisher->add_action( "primal_wrath,target_if=max:dot.bloodseeker_vines.ticking,if=spell_targets.primal_wrath>1&((dot.primal_wrath.remains<6.5&!buff.bs_inc.up|dot.primal_wrath.refreshable)|(!talent.rampant_ferocity.enabled&(spell_targets.primal_wrath>1&!dot.bloodseeker_vines.ticking&!buff.ravage.up|spell_targets.primal_wrath>6+talent.ravage)))","maintain/upgrade pws, if we have 6.5+s left on pw, we will instead bite if we have rampant ferocity talented. Without rampant, we will bite a vined target with 6 or fewer targets. If we have Ravage, we use specifically Ravage at 7 or fewer targets." );
finisher->add_action( "rip,target_if=refreshable,if=(!talent.primal_wrath|spell_targets=1)&(buff.bloodtalons.up|!talent.bloodtalons)&(buff.tigers_fury.up|dot.rip.remains<cooldown.tigers_fury.remains)","rip if single target or pw isnt up. Rip with bloodtalons if talented. If tigers fury will be up before rip falls off, then delay refresh" );
finisher->add_action( "pool_resource,for_next=1" );
finisher->add_action( "ferocious_bite,max_energy=1,target_if=max:dot.bloodseeker_vines.ticking,if=!buff.bs_inc.up|!talent.soul_of_the_forest.enabled" );
finisher->add_action( "ferocious_bite,target_if=max:dot.bloodseeker_vines.ticking" );

variable->add_action( "variable,name=effective_energy,op=set,value=energy+(40*buff.clearcasting.stack)+(3*energy.regen)+(50*(cooldown.tigers_fury.remains<3.5))","most expensive bt cycle is Shred + Thrash + Rake, 40+40+35 for 115 energy. During incarn it is 32+32+28 for 92energy" );
variable->add_action( "variable,name=time_to_pool,op=set,value=((115-variable.effective_energy-(23*buff.incarnation.up))%energy.regen)","estimated time until we have enough energy to proc bloodtalons." );
variable->add_action( "variable,name=need_bt,value=talent.bloodtalons&buff.bloodtalons.stack<=1","try to proc bt if we have 1 or 0 stacks of bloodtalons" );
variable->add_action( "variable,name=cc_capped,value=buff.clearcasting.stack=(1+talent.moment_of_clarity)","capped on clearcasting stacks" );
variable->add_action( "variable,name=lastConvoke,value=(cooldown.convoke_the_spirits.remains+cooldown.convoke_the_spirits.duration)>remains&cooldown.convoke_the_spirits.remains<remains","checks if theres exactly 1 convoke remaining in sim" );
variable->add_action( "variable,name=lastZerk,value=(cooldown.bs_inc.remains+cooldown.bs_inc.duration+5)>remains&cooldown.convoke_the_spirits.remains<remains","checks if theres exactly 1 berserk cast remaining in sim                 : at least 5s spare for now, TODO: check #s" );
variable->add_action( "variable,name=lastPotion,value=(cooldown.potions.remains+cooldown.potions.duration+15)>remains&cooldown.potions.remains+15<remains","approximates if theres exactly 1 potion cast remaining with duration to spare" );
variable->add_action( "variable,name=zerk_biteweave,op=reset","optional variable that alternates bite/pw during berserk regardless of talent setup. Off by default" );
variable->add_action( "variable,name=regrowth,op=reset","optional variable that sends regrowth and renewal casts. Turned off by default" );
variable->add_action( "variable,name=easy_swipe,op=reset","optional variable that forgoes shredding in AoE. Turned off by default" );