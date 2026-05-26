
class ConstraintValidator {

    static prioritize(orders){
        return [...orders].sort((a,b)=>{
            if(b.priority !== a.priority){
                return b.priority - a.priority;
            }

            return a.deadline.localeCompare(b.deadline);
        });
    }

    static checkCapacity(weight,max){
        return weight <= max;
    }

    static checkFuel(distance,fuelLimit){
        return distance <= fuelLimit;
    }

    static priorityName(priority){
        if(priority === 3) return 'VIP';
        if(priority === 2) return 'MEDIUM';
        return 'LOW';
    }
}
