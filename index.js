var c = require('./build/Release/NumScript.node')
//generator function for internal id generation
id=0;
const newId=()=>id++;
//todo implement multi dimensional vectors
class Vector{
    constructor(data,dim){

        if(dim == undefined){
            dim = data.length;
            //calculate dim
        }
        this.dim=dim;
//        c.newVector(this.id,data);
        this.id = newId();
        c.newVector(this.id,data);
    }
    add(other){
        //if other is a vector add it element wise to this
        //use switch case
        switch(other.constructor.name){
            case "Vector":
                //if other is the same dim as this add it element wise
                if(other.dim == this.dim){
                    c.addVector(this.id,other.id, this.dim);
                }
                else{
                throw new Error("Dimension mismatch " + "this.dim: " + this.dim + " other.dim: " + other.dim);
                }
                break;
            case "Number":
                //if other is a number add it to all elements of this
                c.addNumber(this.id,other);
                break;

    }
}

dot(other){
        //if other is a vector add it element wise to this
        //use switch case
  
           
                //if other is the same dim as this add it element wise
                if(other.dim == this.dim){
                    return c.dotProd(this.id,other.id, this.dim);
                }
                
              

}
cross(other){
    if(other.dim == this.dim){
        return c.crossProd(this.id,other.id, this.dim);
    }
    throw new Error("Dimension mismatch " + "this.dim: " + this.dim + " other.dim: " + other.dim);
}

 mult(other){
        //if other is a vector add it element wise to this
        //use switch case
        switch(other.constructor.name){
            case "Vector":
                //if other is the same dim as this add it element wise
                if(other.dim == this.dim){
                    c.multVector(this.id,other.id);
                }
                else{
                throw new Error("Dimension mismatch " + "this.dim: " + this.dim + " other.dim: " + other.dim);
                }
                break;
            case "Number":
                //if other is a number add it to all elements of this
                c.multNumber(this.id,other);
                break;

    }

    
}
    getVector(){
        return c.getVector(this.id, this.dim);
    }

}

 x = new Vector([1,2,3]);
y =  new Vector([-7,8,9]);

console.log("time " + Date.now());
console.log(x.cross(y))
console.log("time " + Date.now());
console.log(x.getVector())

