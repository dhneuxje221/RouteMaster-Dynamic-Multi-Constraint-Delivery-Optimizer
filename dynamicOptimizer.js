
class DynamicOptimizer {

    constructor(){
        this.memo = {};
    }

    distance(a,b){
        let key = `${a.id || 'depot'}-${b.id || 'depot'}`;

        if(this.memo[key]){
            return this.memo[key];
        }

        let dist = haversineDistance(a.lat,a.lng,b.lat,b.lng);
        this.memo[key] = dist;

        return dist;
    }

    bestInsertion(route,newStop,depot){
        if(route.length === 0){
            return [newStop];
        }

        let bestRoute = [...route,newStop];
        let bestCost = Infinity;

        for(let i=0;i<=route.length;i++){
            let copy = [...route];
            copy.splice(i,0,newStop);

            let cost = 0;
            let current = depot;

            for(let stop of copy){
                cost += this.distance(current,stop);
                current = stop;
            }

            if(cost < bestCost){
                bestCost = cost;
                bestRoute = copy;
            }
        }

        return bestRoute;
    }
}
