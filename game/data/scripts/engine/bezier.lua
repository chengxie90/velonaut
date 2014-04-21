Bezier = class()

function Bezier:_init(p0, p1, p2, p3)
	self.p0 = p0
	self.p1 = p1
	self.p2 = p2
	self.p3 = p3
end

function Bezier:getPoint(t)
	return ((1-t)*(1-t)*(1-t)*self.p0) + (3*(1-t)*(1-t)*t*self.p1) + (3*(1-t)*t*t*self.p2) + (t*t*t*self.p3)
end

function Bezier:getDerivativePoint(t)
	return (3*(1-t)*(1-t)*(self.p1-self.p0)) + (6*(1-t)*t*(self.p2-slef.p1)) + (3*t*t*(self.p3-self.p2))
end
