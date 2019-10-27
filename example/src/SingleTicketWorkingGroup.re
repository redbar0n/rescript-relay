module WorkingGroupFragment = [%relay.fragment
  {|
  fragment SingleTicketWorkingGroup_workingGroup on WorkingGroup
    @argumentDefinitions(includeMembers: {type: "Boolean!", defaultValue: false})
    @refetchable(queryName: "SingleTicketWorkingGroupRefetchQuery")
  {
    name
    membersConnection @include(if: $includeMembers) {
      edges {
        node {
          id
          fullName
          ...Avatar_user
        }
      }
    }
  }
|}
];

[@react.component]
let make = (~workingGroup as wgRef) => {
  let (workingGroup, refetch) = WorkingGroupFragment.useRefetchable(wgRef);

  <div>
    <strong> {React.string(workingGroup##name)} </strong>
    <div>
      {switch (workingGroup##membersConnection |> Js.Nullable.toOption) {
       | Some(membersConnection) =>
         membersConnection
         |> ReasonRelayUtils.collectConnectionNodes
         |> Array.map(member =>
              <div key=member##id>
                <em> {React.string(member##fullName)} </em>
              </div>
            )
         |> React.array
       | None =>
         <button
           type_="button"
           onClick={_ =>
             refetch(
               ~variables=
                 WorkingGroupFragment.makeRefetchVariables(
                   ~includeMembers=true,
                   (),
                 ),
               (),
             )
           }>
           {React.string("See members")}
         </button>
       }}
    </div>
  </div>;
};