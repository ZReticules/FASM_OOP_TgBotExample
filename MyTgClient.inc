include "TgClient\TgClient.inc"

struct MyTgClient TgClient
	rnd 		Random64
	patNum 		rptr 1
	patWord 	rptr 1
	patRules	rptr 1

	make override this
ends

proc_noprologue

proc MyTgClient.make c uses pbx, this, apiKey, fpLog
	virtObj .this MyTgClient at pbx from @arg1
	virtObj .parent TgClient at .this
	; int3
	@call .parent->make(@arg2, @arg3)
	@call .this.rnd->make()
	rept 4{
		@call .this.rnd->next()
	}
	@call [.this.patNum] = [SysAllocString](L "^(-?\\d+)(?:\\s+(-?\\d+))?$")
	@call [.this.patWord] = [SysAllocString](L "(?:\\s+([^\\s]+))")
	@call [.this.patRules] = [SysAllocString](L "(.+)[ \t]+\\-\\>[ \t]+(.*)(?:\\n|\\r\\n|\\r)")
	ret
endp

proc MyTgClient.unmake c uses pbx, this
	virtObj .this MyTgClient at pbx from @arg1
	virtObj .parent TgClient at .this
	@call [SysFreeString]([.this.patNum])
	@call [SysFreeString]([.this.patWord])
	@call [SysFreeString]([.this.patRules])
	@call .this.rnd->unmake()
	@call .parent->unmake()
	ret
endp

proc_resprologue
