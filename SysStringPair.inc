proc_noprologue

struct SysStringPair
	key rptr 1
	val rptr 1

	unmake dm this
ends

proc SysStringPair.unmake c uses pbx, this
	virtObj .this SysStringPair at pbx from @arg1
	@call [SysFreeString]([.this.key])
	@call [SysFreeString]([.this.val])
	ret
endp

struct SysStringPairArr
	__count rd 1
	len  	rd 1
	pairs 	rptr 1

	make 	dm this
	unmake	dm this
	append 	dm this
	get 	dm this:inline
ends

proc SysStringPairArr.make c uses pbx, this, cnt
	virtObj .this SysStringPairArr at pbx from @arg1
	@larg pdx, @arg2
	
	mov [.this.__count], edx
	mov [.this.len], 0
	imul edx, edx, sizeof.SysStringPair
	@call [.this.pairs] = CNV::alloc(edx)
	ret
endp

proc SysStringPairArr.unmake c uses pbx psi pdi, this
	virtObj .this SysStringPairArr at pbx from @arg1

	mov edi, [.this.len]
	imul pdi, pdi, sizeof.SysStringPair
	mov pbx, [.this.pairs]
	test pdi, pdi
	@block jz @fb
		add pdi, pbx
		@@:
			sub pdi, sizeof.SysStringPair
			@call SysStringPair::unmake(pdi)
		cmp pdi, pbx
		jne @b
	@endb
	@call CNV::free(pbx)
	ret
endp

proc SysStringPairArr.append c uses pbx, this, key, val
	virtObj .this SysStringPairArr at pbx from @arg1
	@larg pcx, @arg2, pdx, @arg3

	mov eax, [.this.len]
	imul pax, pax, sizeof.SysStringPair
	add pax, [.this.pairs]
	mov [pax + SysStringPair.key], pcx
	mov [pax + SysStringPair.val], pdx
	inc [.this.len]
	ret
endp

macro SysStringPairArr.get this, index{
	@fillGPR pdx, index
	local _this
	inlineObj _this, this, pcx
	imul pdx, pdx, sizeof.SysStringPair
	mov pax, [_this + SysStringPairArr.pairs]
	; lea pax, [pcx + pdx]
	add pax, pdx
}

proc_resprologue
