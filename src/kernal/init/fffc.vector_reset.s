// #LAYOUT# STD *        #TAKE
// #LAYOUT# *   KERNAL_0 #TAKE
// #LAYOUT# *   *        #IGNORE


// $FFFC - CPU Reset Vector
// Uncontrovertial, as it is a CPU requirement, and nothing else.

	.word hw_entry_reset
